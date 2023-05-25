

#include "Application.h"




Application::Application(unsigned int width, unsigned int height)
{
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    Initialize();
}




void Application::Initialize()
{
    // Initialise Camera
    camera = new Camera(glm::vec3(0.0f, 1.0f, 9.0f));

    // Initialise Projection Matrix
    projection = glm::perspective(glm::radians(60.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

    mesh = new Mesh();
    mesh->push_data(vertices, mesh_data, mesh_count);

    second = new Mesh();
    second->push_data(vertices, mesh_data, mesh_count);
    second->translate(2, 2, 2);
}


void Application::Update(float dt, float u_time)
{
    float delta_time = dt/1000;
    float fps = 1000/dt;

    mesh->rotate(0, 0.01, 0);


    // Set View Matrix
    view = camera->GetViewMatrix();


    // Free Movement
    if(Input::GetKey(Input::KEY_W))
        camera->ProcessKeyboard(FORWARD, dt/100);
    if(Input::GetKey(Input::KEY_S))
        camera->ProcessKeyboard(BACKWARD, dt/100);
    if(Input::GetKey(Input::KEY_A))
        camera->ProcessKeyboard(LEFT, dt/100);
    if(Input::GetKey(Input::KEY_D))
        camera->ProcessKeyboard(RIGHT, dt/100);



    if(Input::GetKey(Input::KEY_UP))
        camera->ProcessKeyboard(FORWARD, dt/1500);
    if(Input::GetKey(Input::KEY_DOWN))
        camera->ProcessKeyboard(BACKWARD, dt/1500);
    if(Input::GetKey(Input::KEY_LEFT))
        camera->ProcessKeyboard(LEFT, dt/1500);
    if(Input::GetKey(Input::KEY_RIGHT))
        camera->ProcessKeyboard(RIGHT, dt/1500);




    // Mouse Movement
    if(Input::GetKey(Input::KEY_ESCAPE))
    {
        Input::SetCursor(true);
        mouse_locked = false;
    }

    if(mouse_locked)
    {
        glm::vec2 centerPosition = glm::vec2((float)(SCREEN_WIDTH/2.0f), (float)(SCREEN_HEIGHT/2.0f));
        glm::vec2 deltaPos = Input::GetMousePosition() - centerPosition;
        
        bool rotY = deltaPos.x != 0;
        bool rotX = deltaPos.y != 0;

        camera->ProcessMouseMovement(deltaPos.x * 0.5f, -deltaPos.y * 0.5f);

        // std::cout << "|" << Input::GetMousePosition().x << " , " << Input::GetMousePosition().y << "|";

        if(rotY || rotX)
            Input::SDLSetMousePosition((int)centerPosition.x, (int)centerPosition.y);
    }

    if(Input::GetMouseDown(Input::LEFT_MOUSE))
    {
        glm::vec2 centerPosition = glm::vec2((float)SCREEN_WIDTH/2.0f, (float)SCREEN_HEIGHT/2.0f);
        Input::SetCursor(false);
        Input::SDLSetMousePosition((int)centerPosition.x, (int)centerPosition.y);

        mouse_locked = true;
    }


    Input::Update();


    mesh->sync(mesh_data);
    second->sync(mesh_data);
}




Application::~Application()
{
    delete mesh;
    delete second;

    delete camera;
}