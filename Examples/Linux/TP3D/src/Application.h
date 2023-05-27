
#pragma once

//  Mathematics library
#include <glm/glm.hpp>


//  Engine Headers
#include <obs/camera.h>
#include <obs/orthographic_camera.h>
#include <obs/geometry/plane_ground.h>
#include <obs/skybox.h>
#include <obs/light.h>
#include <obs/obj_model.h>
#include <obs/gltf_model.h>
#include <obs/sprite.h>
#include <obs/text.h>

#include "player.h"



class Application
{
public:

    // constructor/destructor
    Application(unsigned int screen_width, unsigned int screen_height);
    ~Application();

    // initialize game state
    void Init();
    
    // game loop
    void Update(float dt, float uTime);
    void Render(float dt, float uTime);


private:

    unsigned int SCREEN_WIDTH, SCREEN_HEIGHT;

    float ambience = 0.5f;

    const char* shine_diffuse = "res/textures/shine_diffuse.jpg";
    const char* shine_normal = "res/textures/shine_normal.jpg";

    const char* gltf_cube = "gltf/skinning/Woman.gltf";        


    glm::mat4 view;
    glm::mat4 projection;
    // Images that Make up the Skybox(Order is very important!)
    vector<std::string> faces
    {
        // X-AXIS
        "res/sky/right.jpg",
        "res/sky/left.jpg",

        // Y-AXIS
        "res/sky/top.jpg",
        "res/sky/bottom.jpg",

        // Z-AXIS
        "res/sky/front.jpg",
        "res/sky/back.jpg"
    };

    Skybox skybox;

    // camera
    Camera camera;
    OrthographicCamera ortho_camera;

    Light *light;

    PlaneGround floor;
    Sprite sprite;

    ObjModel *monkey;
    Player player;

    Text text;


    gltfModel model;
};
