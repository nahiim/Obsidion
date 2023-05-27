

#pragma once

#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <obs/object.h>




class Camera
{
private:
    
    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    // camera Attributes
    glm::vec3 cam_front;
    glm::vec3 cam_up;
    glm::vec3 cam_right;

    float distance, horizontal_distance, vertical_distance;
    
    float pitch, yaw, roll;

    Object *object;

    
public:
    
    glm::vec3 position;



    Camera()
    {}

    Camera(Object *object, float horizontal_distance, float vertical_distance)
    {
        this->object = object;
        this->horizontal_distance = horizontal_distance;
        this->vertical_distance = vertical_distance;

        position = glm::vec3(0, vertical_distance, horizontal_distance);

        init();
        
        updateCameraVectors();
    }


    void update(float uTime)
    {
        float theta = object->getOrientation().y;
        float offetX = horizontal_distance * sin(glm::radians(theta));
        float offetZ = horizontal_distance * cos(glm::radians(theta));

        position.x = object->getPosition().x + offetX;
        position.z = object->getPosition().z + offetZ;
        position.y = object->getPosition().y + vertical_distance;

        yaw = -90 - object->getOrientation().y;

        updateCameraVectors();
    }


    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(position, position + cam_front, cam_up);
    }


private:



    void init()
    {
        pitch = atan(vertical_distance / horizontal_distance);
        yaw = -90;
        distance = glm::sqrt(glm::pow(vertical_distance, 2) + glm::pow(horizontal_distance, 2));
    }


    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        glm::vec3 front;

        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cam_front = glm::normalize(front);
 
        cam_front = glm::normalize(object->getPosition() - position);
        std::cout << object->getPosition().x << " " << object->getPosition().y << " " << object->getPosition().z << "\n";


        // also re-calculate the Right and Up vector
        // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        cam_right = glm::normalize(glm::cross(cam_front, world_up));
        cam_up    = glm::normalize(glm::cross(cam_right, cam_front));
    }
};

