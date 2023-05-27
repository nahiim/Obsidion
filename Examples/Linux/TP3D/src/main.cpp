
//Using SDL, SDL OpenGL, GLEW, standard IO, and strings
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>


#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


#include "Application.h"

// void SDLSetMousePosition(int x, int y);

//Screen dimension constants
unsigned int SCREEN_WIDTH = 640;
unsigned int SCREEN_HEIGHT = 480;

Application game(SCREEN_WIDTH, SCREEN_HEIGHT);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

float dt, t1, t2 = 0;

float startTime, uTime;

//OpenGL context
SDL_GLContext gContext;

// Main loop flag
bool quit = false;


bool mouseLocked = false;


int main(int argc, char* args[])
{
    // Initialize SDL
    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER );

    SDL_GameController *controller = nullptr;

    for(int i=0 ; i<SDL_NumJoysticks() ; i++)
    {
        if (SDL_IsGameController(i))
        {
            controller = SDL_GameControllerOpen(i);
        }
    }

    //Use OpenGL 3.3 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    //Create window
    gWindow = SDL_CreateWindow
    (
        "Third Person",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    //Create context
    gContext = SDL_GL_CreateContext( gWindow );

    //Initialize GLEW
    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();
    
    //Use Vsync
    SDL_GL_SetSwapInterval( 1 );

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);


    startTime = SDL_GetTicks();

    game.Init();

    //While application is running
    while( !quit )
    {
        t1 = SDL_GetTicks();
        dt = t1 - t2;
        t2 = t1;

        uTime = SDL_GetTicks() - startTime;

        game.Update(dt, uTime);
        game.Render(dt, uTime);

        //User requests quit
        if(Input::GetKey(Input::KEY_Q))
        {
            quit = true;
        }
    
        
        //Update screen
        SDL_GL_SwapWindow( gWindow );
    }

    //Disable text input
    SDL_StopTextInput();

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    if(controller != NULL)
    {
        SDL_GameControllerClose(controller);
    }

    controller = nullptr;

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}