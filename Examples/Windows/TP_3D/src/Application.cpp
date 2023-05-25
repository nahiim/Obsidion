

#include "Application.h"



Application::Application(unsigned int screen_width, unsigned int screen_height)
    : SCREEN_WIDTH(screen_width), SCREEN_HEIGHT(screen_height)
{}


void Application::Init()
{
    // Initialise Projection Matrix
    projection = glm::perspective(glm::radians(60.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 1000.0f);

    light = new Light(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));


    // Initialise skybox
    skybox = Skybox(view, projection, faces);

    floor = PlaneGround(shine_diffuse, shine_normal);
    floor.setProjection(projection);
    floor.scale(100, 100, 100);

    monkey = new ObjModel("res/models/monkey3.obj", "res/textures/bricks.jpg");
    monkey->setProjection(projection);
    monkey->rotate(0, 180, 0);
    // monkey->translate(0,20,0);

    player = Player(monkey);

    model = gltfModel(gltf_cube);
    model.setProjection(projection);
    model.translate(0, 0, -5);

    // Initialise Cameras
    camera = Camera(&player, 10, 3);
}


void Application::Update(float dt, float uTime)
{
    // dt: milliseconds passed since last frame
    // uTime: milliseconds passed since window initialisation
    float deltaTime = dt/1000;    // seconds passed since last frame
    float fps = 1000/dt;          // frames per second

    Input::Update();
    if(Input::GetKey(Input::KEY_Q))
    {
        quit = true;
    }

    player.move(deltaTime);

    camera.update(uTime);

    cout << fps << " FPS" << "\n";
}

void Application::Render(float dt, float uTime)
{
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    skybox.Draw(camera);

    floor.Draw(camera, light, ambience);

    monkey->Draw(camera, light, ambience);

    model.draw(camera);
}




Application::~Application()
{
    delete monkey;
}