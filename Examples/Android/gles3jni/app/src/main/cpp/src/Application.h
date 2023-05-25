
#pragma once

#include <android/log.h>

#include <GLES3/gl32.h>

//  Mathematics library
#include "../include/glm/glm.hpp"

//  Engine Headers
#include "../include/obs/orthographic_camera.h"
#include "../include/obs/geometry/rectangle.h"
#include "../include/obs/sprite.h"


class Application
{
public:

    // constructor/destructor
    Application();
    ~Application();
    Application(unsigned int screen_width, unsigned int screen_height);

    // initialize game state
    void Init(AssetLoader asset_loader);
    
    // game loop
    void Update(double dt, double time_passed);
    void Render(double dt, double time_passed);

    bool quit = false;


private:

    unsigned int SCREEN_WIDTH = 720, SCREEN_HEIGHT = 1600;

    AssetLoader m_asset_loader;

    OrthographicCamera ortho_camera;

    Rectangle rect;

    Sprite sprite;
};