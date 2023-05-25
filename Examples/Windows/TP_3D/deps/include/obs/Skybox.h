
#pragma once

#include <gl/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <obs/Shader.h>
#include <obs/Vertex.h>
#include <obs/Camera.h>

#include <string>
#include <vector>

using namespace std;


class Skybox
{

public:

    Skybox();
    Skybox(glm::mat4 view, glm::mat4 projection, vector<std::string> faces);
    void Draw(Camera camera);


private:

    unsigned int VBO, VAO, cubemapTexture;
    glm::mat4 view, projection;

    Shader shader;
    
	unsigned int loadCubemap(vector<std::string> faces);

    float vertices[180] =
    {
        // positions                textcoords
        -1.0f, -1.0f, -1.0f,        0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,        1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,        1.0f, 1.0f,
         1.0f,  1.0f, -1.0f,        1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,        0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,        0.0f, 0.0f,

        -1.0f, -1.0f,  1.0f,        0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,        1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,        0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,        0.0f, 0.0f,

        -1.0f,  1.0f,  1.0f,        1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,        1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,        0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,        0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,        0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,        1.0f, 0.0f,

         1.0f,  1.0f,  1.0f,        1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,        1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,        0.0f, 1.0f,
         1.0f, -1.0f, -1.0f,        0.0f, 1.0f,
         1.0f, -1.0f,  1.0f,        0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 0.0f,

        -1.0f, -1.0f, -1.0f,        0.0f, 1.0f,
         1.0f, -1.0f, -1.0f,        1.0f, 1.0f,
         1.0f, -1.0f,  1.0f,        1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,        1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,        0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,        0.0f, 1.0f,

        -1.0f,  1.0f, -1.0f,        0.0f, 1.0f,
         1.0f,  1.0f, -1.0f,        1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,        0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,        0.0f, 1.0f
    };
};