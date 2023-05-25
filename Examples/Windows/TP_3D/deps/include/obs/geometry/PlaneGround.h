

#pragma once


#include "stb_image.h"

#include <gl/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <obs/Shader.h>
#include <obs/Light.h>

#include <string>
#include <vector>



class PlaneGround
{
    public:

        // Default Constructor
        PlaneGround(){}


        // constructor
        PlaneGround(const std::string& texturePath, const std::string& normalPath = "null")
        {
            this->model = glm::mat4(1.0f);
            
            diffuseID = LoadTexture(texturePath);

            if (normalPath != "null")
            {
                normalID = LoadTexture(normalPath);
            }

            fillGrid();

            // now that we have all the required data, set the vertex buffers and its attribute pointers.
            setupTerrain(positions, texCoords, normals);
        }

        void setProjection(glm::mat4 projection)
        {
            shader = new Shader("res/shaders/plane_ground_vert.glsl", "res/shaders/plane_ground_frag.glsl");
            shader->use();
            shader->setMat4("projection", projection);

            shader->setInt("diffuseMap", 0);
            shader->setInt("normalMap", 1);
        }

        void rotate(float x, float y, float z)
        {
            glm::vec3 eular = glm::vec3(
                glm::radians(x),
                glm::radians(y),
                glm::radians(z)
            );
            glm::quat rotation = glm::quat(eular);

            glm::mat4 rotationMatrix = glm::toMat4(rotation);
            model = model * rotationMatrix;
        }

        void translate(float x, float y, float z)
        {
            glm::mat4 identityMatrix = glm::mat4(1.0f);
            glm::mat4 translationMatrix = glm::translate(identityMatrix, glm::vec3(x, y, z));
            model = model * translationMatrix;
        }

        void scale(float x, float y, float z)
        {
            glm::mat4 identityMatrix = glm::mat4(1.0f);
            glm::mat4 scaleMatrix = glm::scale(identityMatrix, glm::vec3(x, y, z));
            model = model * scaleMatrix;
        }


        glm::vec4 getColor()
        {
            return this->color;
        }

        void setColor(glm::vec4 color)
        {
            this->color = color;
        }

        void setColor(float red, float green, float blue, float alpha)
        {
            this->color.x = red;
            this->color.y = green;
            this->color.z = blue;
            this->color.w = alpha;
        }

        // render the mesh
        void Draw(Camera camera, Light *light, float ambience)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseID);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normalID);

            shader->use();
            shader->setMat4("view", camera.GetViewMatrix());
            shader->setMat4("model", model);


            shader->setVec3("lightPosition", light->position);
            shader->setVec3("lightColor", light->color);
            shader->setFloat("ambience", ambience);
            shader->setVec3("camPos", camera.position);


            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, IND, GL_UNSIGNED_INT, NULL);
            glBindVertexArray(0);
            glActiveTexture(GL_TEXTURE0);
        }


        unsigned int LoadTexture(const std::string& texturePath)
        {
            unsigned int textureID;

            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
            int width, height, nrChannels;

            unsigned char *data = stbi_load((texturePath).c_str(), &width, &height, &nrChannels, 0);

            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }

            stbi_image_free(data);

            return textureID;
        }



    private:

        glm::mat4 model;

        unsigned int diffuseID, normalID;

        // render data 
        unsigned int VAO;
        unsigned int VBO[3];

        GLuint tangentBuffer;
        GLuint bitangentBuffer;

        unsigned int n = 50;

        unsigned int vertSize = n*n;

        glm::vec3* positions = new glm::vec3[vertSize];
        glm::vec2* texCoords = new glm::vec2[vertSize];
        glm::vec3* normals   = new glm::vec3[vertSize];

        std::vector<glm::vec3> tangents;

        unsigned int IND = 6 * (n-1) * (n-1);   // Number of indices
        unsigned int* indices = new unsigned int[IND];

        glm::vec3* positionPtr = positions;
        glm::vec2* texCoordPtr = texCoords;
        glm::vec3* normalPtr   = normals;
        unsigned int* indexPtr = indices;

        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec4 color;

        Shader* shader;



        void fillGrid()
        {
            for(unsigned int x=0 ; x<n ; x++) for(unsigned int z=0; z<n ; z++)
            {
                *positionPtr++ = glm::vec3( (z / (float)n) - 0.5f, 0, (x / (float)n) - 0.5f);

                *texCoordPtr++ = glm::vec2((z / (float)n) * n, (x / (float)n) * n);

                *normalPtr++ = upVector;
            }

            for(unsigned int x=0 ; x<n-1 ; x++) for(unsigned int z=0; z<n-1 ; z++)
            {
                *indexPtr++ = (x * n) + z;          
                *indexPtr++ = ((x + 1) * n) + z;    
                *indexPtr++ = ((x + 1) * n) + z + 1;

                *indexPtr++ = (x * n) + z;          
                *indexPtr++ = ((x + 1) * n) + z + 1;
                *indexPtr++ = (x * n) + z + 1;
            }

            computeTangents();
        }



        // initializes all the buffer objects/arrays
        void setupTerrain(glm::vec3* positions, glm::vec2* texCoords, glm::vec3* normals)
        {
            // create our vertex array object
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glGenBuffers(6, VBO);

            // our terrain vertex positions are known so we can set those right away
            glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertSize, positions, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

            // we've also computed our base texture coordinates, so fill those in
            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertSize, texCoords, GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

            // our normals are also computed, so pass those in too
            glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertSize, normals, GL_STATIC_DRAW);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

            glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertSize, &tangents[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

            // our terrain indices define how our terrain should be constructed using the vertices defined above
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[5]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * IND, indices, GL_STATIC_DRAW);

            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }







        void computeTangents()
        {
            for ( int i=0; i<vertSize; i+=3)
            {
                // Shortcuts for UVs
                glm::vec2 uv0 = texCoords[i+0];
                glm::vec2 uv1 = texCoords[i+1];
                glm::vec2 uv2 = texCoords[i+2];

                // UV delta
                glm::vec2 deltaUV1 = uv1-uv0;
                glm::vec2 deltaUV2 = uv2-uv0;
                
                glm::vec3 tangent = glm::vec3(deltaUV1.x, deltaUV1.y, 0.0f);


                // Set the same tangent for all three vertices of the triangle.
                tangents.push_back(tangent);
                tangents.push_back(tangent);
                tangents.push_back(tangent);
            }
        }
};