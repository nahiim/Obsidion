
#pragma once

#include <iostream>
#include <vector>

#include <obs/mesh.h>
#include <obs/input.h>
#include <obs/camera_free.h>


class Application 
{
public:

    glm::mat4 view;
    glm::mat4 projection;


    // constructor/destructor
    Application(unsigned int width, unsigned int height);
    ~Application();

    // initialize 
    void Initialize();
    
    // game loop
    void Update(float dt, float u_time);


    std::vector<Vertex> vertices;
    std::vector<glm::mat4> mesh_data;
    unsigned int mesh_count;

    std::vector<std::vector<Vertex>> &vertices;
    std::vector<std::vector<glm::mat4>> &mesh_data;
    std::vector<unsigned int> &mesh_count;


private:

    unsigned int SCREEN_WIDTH, SCREEN_HEIGHT;
    bool mouse_locked = false;

    Mesh *mesh;
    Mesh *second;
    Camera *camera;
};
