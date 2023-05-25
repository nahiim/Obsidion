
#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



class OrthographicCamera
{

private:
    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;


public:
	// Default constructor
	OrthographicCamera()
	{}


    // Constructor - specify left, right, bottom, top, near plane and far plane values
    OrthographicCamera(float left, float right, float bottom, float top, float near_plane = -1.0f, float far_plane = 1.0f)
    {
    	projection_matrix = glm::ortho(left, right, bottom, top, near_plane, far_plane);
    	view_matrix = glm::mat4(1.0f);
    }

    // move camera
    void translate(const float& x, const float& y)
    {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
        view_matrix *= transform;
	}

    // rotate camera (in degrees)
    void rotate(const float& rotation)
    {
		glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));
    	view_matrix *= transform;
    }

    // Get the view matrix of the camera
    glm::mat4 getViewMatrix() const
    {
    	return view_matrix;
    }

    // Get the projection matrix of the camera
    glm::mat4 getProjectionMatrix() const
    {
    	return projection_matrix;
    }
};