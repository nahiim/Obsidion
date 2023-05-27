

#pragma once


#include "stb_image.h"

#include <GL/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#include <obs/shader.h>

#include <string>
#include <vector>



class Sphere {

    public:

        // Default Constructor
        Sphere(){}


        // constructor
        Sphere(glm::mat4 view, glm::mat4 projection)
        {
            this->view = view;
            this->projection = projection;

            this->model = glm::mat4(1.0f);

            // Compute Vertex positions and normals
            fillGrid();

            // now that we have all the required data, set the vertex buffers and its attribute pointers.
            setupMesh(positions, normals); 
        }

        void setProjection(glm::mat4 projection)
        {
            this->projection = projection;

            shader = new Shader("res/shaders/smoothSH_vert.glsl", "res/shaders/smoothSH_frag.glsl");
            shader->use();
            shader->setMat4("projection", projection);
        }

        void rotate(float dt, glm::vec3 amount)
        {
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, dt, amount);
            model = model * trans;
        }

        void translate(glm::vec3 amount)
        {
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, amount);
            model = model * trans;
        }

        void scale(float x, float y, float z)
        {
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::scale(trans, glm::vec3(x, y, z));
            model = model * trans;
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
        void Draw(Camera camera, Light* light, float ambience)
        {
            shader->use();
            view = camera.GetViewMatrix();
            shader->setMat4("view", view);


            // retrieve the matrix uniform locations
            // unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
            // // pass them to the shaders
            // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            shader->setMat4("model", model);

            // // retrieve color location
            // unsigned int colorLoc = glGetUniformLocation(shader->ID, "aColor");
            // // pass color to shader
            // glUniform4f(colorLoc, color.x, color.y, color.z, color.w);

            shader->setVec4("aColor", color);

            shader->setVec3("lightPosition", light->position);
            shader->setVec3("lightColor", light->color);
            shader->setFloat("ambience", ambience);
            shader->setVec3("camPos", camera.position);


            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, (slices * stacks + slices) * 6, GL_UNSIGNED_INT, NULL);
        }


    private:

        glm::mat4 model, view, projection;

        // unsigned int m_texture;



        // render data
        unsigned int VAO;
        unsigned int VBO[3];

        unsigned int stacks = 30, slices = 30;
        // float radius = 2.0f;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;

        std::vector<unsigned int> indices;

        glm::vec4 color;

        Shader* shader;



        void fillGrid()
        {
            // Compute The Vertices
            for (int i = 0; i <= stacks; ++i)
            {
                float V   = i / (float) stacks;
                float phi = V * glm::pi <float> () - glm::pi <float> () / 2.0;

                // Loop Through Slices
                for (int j = 0; j <= slices; ++j)
                {
                    float U = j / (float) slices;
                    float theta = U * (glm::pi <float> () * 2);

                    // Calc The Vertex Positions
                    float x = cos(theta) * cos(phi);
                    float y = cos(theta) * sin(phi);
                    float z = sin(theta);

                    // Push Back Vertex Data
                    positions.push_back (glm::vec3 (x, y, z));
                    normals.push_back(glm::vec3(x, y, z));
                // textureCoords.push_back( glm::vec2(U, V) );
                }
            }

            int noPerSlice = slices + 1;
            for(int i=0; i < stacks; ++i)
            {
                for (int j = 0; j < slices; ++j)
                {
                    int start_i = (i * noPerSlice) + j;

                    indices.push_back( start_i );
                    indices.push_back( start_i + noPerSlice + 1 );
                    indices.push_back( start_i + noPerSlice );

                    indices.push_back( start_i + noPerSlice + 1 );
                    indices.push_back( start_i );
                    indices.push_back( start_i + 1 );
                }
            }
        }



        // initializes all the buffer objects/arrays
        void setupMesh(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals)
        {
            // create our vertex array object
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glGenBuffers(3, VBO);

            // our sphere vertex positions are known so we can set those right away
            glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), &positions[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

            // our normals are also computed, so pass those in too
            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

            // our sphere indices define how our sphere should be constructed using the vertices defined above
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[2]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
};