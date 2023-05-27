
#pragma once

#include "stb_image.h"

#include <gl/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#include <obs/Shader.h>


class Sprite
{
    public:

        // Default Constructor
        Sprite(){}


        // constructor
        Sprite(const char* path)
        {
            loadTexture(path);

            // set the vertex buffers and its attribute pointers.
            setupQuad();
        }

        ~Sprite()
        {}

        void setProjection(const glm::mat4& projection)
        {
            shader = new Shader("res/shaders/sprite_vert.glsl", "res/shaders/sprite_frag.glsl");
            shader->use();
            shader->setMat4("projection", projection);
            // shader->setInt("sprite_texture", 0);
        }

        void rotate(const float& angle)
        {
            // create a rotation matrix around the Z axis
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

            // apply the rotation to the billboard's vertices
            for (int i = 0; i < 4; i++)
            {
                glm::vec4 vertex(quad_positions[i * 3], quad_positions[i * 3 + 1], 0.0f, 1.0f);
                vertex = rotation * vertex;
                quad_positions[i * 2] = vertex.x;
                quad_positions[i * 2 + 1] = vertex.y;
            }
        }

        void translate(const float& x, const float& y)
        {
            for (int i = 0; i < 4; i++)
            {
                quad_positions[i * 3]     += x; // X position
                quad_positions[i * 3 + 1] += y; // Y position
            }

            glBindBuffer(GL_ARRAY_BUFFER, quadVBO[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, quad_positions, GL_STATIC_DRAW);
        }

        void scale(const float& scale_factor)
        {
            for (int i = 0; i < 4; i++)
            {
                quad_positions[i * 3]     *= scale_factor; // X position
                quad_positions[i * 3 + 1] *= scale_factor; // Y position
            }

            glBindBuffer(GL_ARRAY_BUFFER, quadVBO[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, quad_positions, GL_STATIC_DRAW);
        }

        // render the mesh
        void draw(const OrthographicCamera& camera)
        {
            shader->use();

            // bind textures on corresponding texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glBindVertexArray(quadVAO);
 
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }


    private:

        glm::mat4 m_projection;
        // render data
        unsigned int quadVBO[3], quadVAO;

        unsigned char* image;
        GLuint textureID;

        Shader* shader;

        // Create a 2D quad
        float quad_positions[3 * 4] = {
            // position
             0.5f,  0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };
        float quad_texcoords[2 * 4] = {
            // texture coordinates
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f
        };
        unsigned int quad_indices[6] = {
            0, 1, 3,
            1, 2, 3
        };


        void loadTexture(const char* path)
        {
            // Generate a texture
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
         
            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Load the sprite image
            int width, height, channels_count;
            image = stbi_load(path, &width, &height, &channels_count, 0);

            // Upload the image data to the texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            // glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(image);
        }



        // initializes all the buffer objects/arrays
        void setupQuad()
        {
            // Create a vertex buffer object for the quad
            glGenVertexArrays(1, &quadVAO);
            glBindVertexArray(quadVAO);

            glGenBuffers(3, quadVBO);

            glBindBuffer(GL_ARRAY_BUFFER, quadVBO[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, quad_positions, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            glBindBuffer(GL_ARRAY_BUFFER, quadVBO[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 4, quad_texcoords, GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadVBO[2]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, quad_indices, GL_STATIC_DRAW);
        }
};