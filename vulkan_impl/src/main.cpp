

#include <iostream>
#include <cstdlib>

#include <jvk/engine.h>

#include "Application.h"


Engine *engine;

Application *app;

bool running = true;


int main(int argc, char *args[])
{
    engine = new Engine();

    app = new Application(
        engine->window->SCREEN_WIDTH,
        engine->window->SCREEN_HEIGHT
    );

    engine->init(app->vertices, app->mesh_data);


    while (running)
    {
        engine->window->loop(running);

        app->Update(engine->window->dt, engine->window->u_time);

        engine->render(
            app->mesh_data,
            app->view,
            app->projection
        );
    }


    delete app;
    delete engine;


    // std::cout << "\n\nPRESS ENTER TO EXIT!  \n";
    // getchar();
    system("pause");



    return 0;
}