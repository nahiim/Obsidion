
#pragma once


#include <gl/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#include <obs/Shader.h>


class Rectangle
{
public:

    // Default Constructor
    Rectangle() {}


    // constructor
    Rectangle(float red, float green, float blue)
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
        // set the vertex buffers and its attribute pointers.
        setupQuad();
    }

    ~Rectangle()
    {}

    void setProjection(const glm::mat4& projection)
    {
        shader = new Shader("res/shaders/shape_vert.glsl", "res/shaders/shape_frag.glsl");
        shader->use();
        shader->setMat4("projection", projection);
    }

    void rotate(const float& angle_degrees)
    {
        // Compute the center of the quad
        float center_x = 0.0f, center_y = 0.0f;
        for (int i = 0; i < 4; i++)
        {
            center_x += quad_positions[i * 3];
            center_y += quad_positions[i * 3 + 1];
        }
        center_x /= 4.0f;
        center_y /= 4.0f;

        // Convert angle to radians
        float angle_radians = glm::radians(angle_degrees);

        // Compute sine and cosine of angle
        float sin_angle = sin(angle_radians);
        float cos_angle = cos(angle_radians);

        // Rotate each vertex around the center of the quad
        for (int i = 0; i < 4; i++)
        {
            // Translate vertex to origin
            float x = quad_positions[i * 3] - center_x;
            float y = quad_positions[i * 3 + 1] - center_y;

            // Apply rotation
            float new_x = x * cos_angle - y * sin_angle;
            float new_y = x * sin_angle + y * cos_angle;

            // Translate vertex back to original position
            quad_positions[i * 3] = new_x + center_x;
            quad_positions[i * 3 + 1] = new_y + center_y;
        }

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, quad_positions, GL_STATIC_DRAW);
    }

    void translate(const float& x, const float& y)
    {
        for (int i = 0; i < 4; i++)
        {
            quad_positions[i * 3] += x; // X position
            quad_positions[i * 3 + 1] += y; // Y position
        }

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, quad_positions, GL_STATIC_DRAW);
    }

    void scale(const float& sx, const float& sy)
    {
        // Calculate the center of the quad
        float cx = (quad_positions[0] + quad_positions[3] + quad_positions[6] + quad_positions[9]) / 4.0f;
        float cy = (quad_positions[1] + quad_positions[4] + quad_positions[7] + quad_positions[10]) / 4.0f;

        // Translate the quad to the origin
        for (int i = 0; i < 4; i++)
        {
            quad_positions[i * 3] -= cx;
            quad_positions[i * 3 + 1] -= cy;
        }

        // Scale each vertex by the specified amount
        for (int i = 0; i < 4; i++)
        {
            quad_positions[i * 3] *= sx; // X position
            quad_positions[i * 3 + 1] *= sy; // Y position
        }

        // Translate the quad back to its original position
        for (int i = 0; i < 4; i++)
        {
            quad_positions[i * 3] += cx;
            quad_positions[i * 3 + 1] += cy;
        }

        // Update the vertex buffer object with the new vertex positions
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, quad_positions, GL_STATIC_DRAW);
    }

    // render the mesh
    void draw(const OrthographicCamera& camera)
    {
        shader->use();

        glBindVertexArray(quadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }


private:

    // render data
    unsigned int quadVBO[3], quadVAO;

    float red, green, blue;

    Shader* shader;

    // Create a 2D quad
    float quad_positions[3 * 4] = {
        // position
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    float quad_colors[3 * 4];
    unsigned int quad_indices[6] = {
        0, 1, 3,
        1, 2, 3
    };



    // initializes all the buffer objects/arrays
    void setupQuad()
    {
        for (int i = 0; i < 4; i++)
        {
            quad_colors[i*3]     = red;
            quad_colors[i*3 + 1] = green;
            quad_colors[i*3 + 2] = blue;
        }

        // Create a vertex buffer object for the quad
        glGenVertexArrays(1, &quadVAO);
        glBindVertexArray(quadVAO);

        glGenBuffers(3, quadVBO);

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, quad_positions, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, quad_colors, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadVBO[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, quad_indices, GL_STATIC_DRAW);
    }
};