
#pragma once

//  Mathematics library
#include <glm/glm.hpp>

#include <obs/audio/soundmanager.h>

//  Engine Headers
#include <obs/orthographic_camera.h>
#include <obs/geometry/rectangle.h>
#include <obs/sprite.h>
#include <obs/text.h>

#include <obs/input.h>

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

    bool quit = false;


private:

    unsigned int SCREEN_WIDTH, SCREEN_HEIGHT;     

    OrthographicCamera ortho_camera;

    Sprite board;
    Sprite ball;
    Rectangle rect;

    Text text;

    // OpenAL buffer object for meadow ambience
    ALuint ambience;
};
