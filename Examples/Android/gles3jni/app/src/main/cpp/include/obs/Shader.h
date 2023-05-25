
#pragma once

#include <GLES3/gl32.h>

#include <glm/glm.hpp>

#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

#include "asset_loader.h"

class Shader
{
public:

    Shader(){}
    unsigned int ID;

    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(AssetLoader asset_loader, const std::string &vertexPath, const std::string &fragmentPath, const char *geometryPath = nullptr)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        const char* vShaderCode = asset_loader.loadAsset(vertexPath);
        const char * fShaderCode = asset_loader.loadAsset(fragmentPath);

        // 2. compile shaders
        unsigned int vertex, fragment;

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if(geometryPath != nullptr)
        {
            const char * gShaderCode = asset_loader.loadAsset(geometryPath);
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if(geometryPath != nullptr)
            glAttachShader(ID, geometry);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if(geometryPath != nullptr)
            glDeleteShader(geometry);

        PopulateAttributes();
        PopulateUniforms();
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }


    void Bind() {
        glUseProgram(ID);
    }

    void UnBind() {
        glUseProgram(0);
    }

    unsigned int GetHandle() {
        return ID;
    }

    unsigned int GetAttribute(const std::string& name) {
        std::map<std::string, unsigned int>::iterator it = mAttributes.find(name);
        if (it == mAttributes.end()) {
            std::cout << "Retrieving bad attribute index: " << name << "\n";
            __android_log_print(ANDROID_LOG_ERROR, "APP", "Retrieving bad attribute index: %s", name.c_str());
            return 0;
        }
        return it->second;
    }

    unsigned int GetUniform(const std::string& name) {
        std::map<std::string, unsigned int>::iterator it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            std::cout << "Retrieving bad uniform index: " << name << "\n";
            __android_log_print(ANDROID_LOG_ERROR, "APP", "Retrieving bad uniform index: %s", name.c_str());
            return 0;
        }
        return it->second;
    }

private:


    std::map<std::string, unsigned int> mAttributes;
    std::map<std::string, unsigned int> mUniforms;


    void PopulateAttributes() {
        int count = -1;
        int length;
        char name[128];
        int size;
        GLenum type;

        glUseProgram(ID);
        glGetProgramiv(ID, GL_ACTIVE_ATTRIBUTES, &count);

        for (int i = 0; i < count; ++i) {
            memset(name, 0, sizeof(char) * 128);
            glGetActiveAttrib(ID, (GLuint)i, 128, &length, &size, &type, name);
            int attrib = glGetAttribLocation(ID, name);
            if (attrib >= 0) {
                mAttributes[name] = attrib;
            }
        }

        glUseProgram(0);
    }

    void PopulateUniforms() {
        int count = -1;
        int length;
        char name[128];
        int size;
        GLenum type;
        char testName[256];

        glUseProgram(ID);
        glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &count);

        for (int i = 0; i < count; ++i) {
            memset(name, 0, sizeof(char) * 128);
            glGetActiveUniform(ID, (GLuint)i, 128, &length, &size, &type, name);

            int uniform = glGetUniformLocation(ID, name);
            if (uniform >= 0) {
                std::string uniformName = name;
                std::size_t found = uniformName.find('[');
                if (found != std::string::npos) {
                    uniformName.erase(uniformName.begin() + found, uniformName.end());
                    // Populate subscripted names too
                    unsigned int uniformIndex = 0;
                    while (true) {
                        memset(testName, 0, sizeof(char) * 256);
                        sprintf(testName, "%s[%d]", uniformName.c_str(), uniformIndex++);
                        int uniformLocation = glGetUniformLocation(ID, testName);
                        if (uniformLocation < 0) {
                            break;
                        }
                        mUniforms[testName] = uniformLocation;
                    }
                }
                mUniforms[uniformName] = uniform;
            }
        }

        glUseProgram(0);
    }








    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                __android_log_print(ANDROID_LOG_ERROR, "APP", "ERROR::SHADER_COMPILATION_ERROR of type: %s\n %s\n", type.c_str(), infoLog);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                __android_log_print(ANDROID_LOG_ERROR, "APP", "ERROR::SHADER_LINKING_ERROR of type: %s\n %s\n", type.c_str(), infoLog);
            }
        }
    }
};