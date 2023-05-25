
//Using SDL, SDL OpenGL, GLEW, standard IO, and strings
#define SDL_MAIN_HANDLED
#include <SDL2\SDL.h>


#include <gl\glew.h>
#include <SDL2\SDL_opengl.h>
#include <gl\glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


#include "Application.h"


//Screen dimension constants
unsigned int SCREEN_WIDTH = 640;
unsigned int SCREEN_HEIGHT = 480;

Application app(SCREEN_WIDTH, SCREEN_HEIGHT);

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
    SDL_Init( SDL_INIT_VIDEO );


    //Use OpenGL 3.3 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    //Create window
    gWindow = SDL_CreateWindow
    (
        "Basic Example",
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

    app.Init();

    //While application is running
    while( !quit )
    {
        t1 = SDL_GetTicks();
        dt = t1 - t2;
        t2 = t1;

        uTime = SDL_GetTicks() - startTime;

        app.Update(dt, uTime);
        app.Render(dt, uTime);


        if(app.quit)
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

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}