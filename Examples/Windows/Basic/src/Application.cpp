

#include "Application.h"



Application::Application(unsigned int screen_width, unsigned int screen_height)
    : SCREEN_WIDTH(screen_width), SCREEN_HEIGHT(screen_height)
{}


void Application::Init()
{
    ortho_camera = OrthographicCamera(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

    rect = Rectangle(0.0f, 1.0f, 1.0f);
    rect.setProjection(ortho_camera.getProjectionMatrix());
    rect.translate((SCREEN_WIDTH / 2) + 100.0f, SCREEN_HEIGHT / 2);
    rect.scale(100.0f, 100.0f);

    board = Sprite("res/textures/board.png");
    board.setProjection(ortho_camera.getProjectionMatrix());
    board.translate(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    board.scale(SCREEN_WIDTH, SCREEN_HEIGHT);

    ball = Sprite("res/smile.jpg");
    ball.setProjection(ortho_camera.getProjectionMatrix());
    ball.translate(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    ball.scale(100.0f, 100.0f);

    text = Text("res/Antonio-Bold.ttf");
    text.setProjection(ortho_camera.getProjectionMatrix());

    // start the ambient meadow sound effect
    ambience = SoundManager::getInstance()->loadWAV("res/wav/ambience.wav");
    SoundManager::getInstance()->loopSound(ambience);
}


void Application::Update(float dt, float uTime)
{
    // dt: milliseconds passed since last frame
    // uTime: milliseconds passed since window initialisation
    float deltaTime = dt/1000;    // seconds passed since last frame
    float fps = 1000/dt;          // frames per second

    Input::Update();

    ball.rotate(1.10f);
    rect.rotate(-1.10f);

    //User requests quit
    if(Input::GetKey(Input::KEY_Q))
    {
        quit = true;
    }
}

void Application::Render(float dt, float uTime)
{
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    text.render("I am not weasel", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

    glDisable(GL_BLEND);

    rect.draw(ortho_camera);
    ball.draw(ortho_camera);
    board.draw(ortho_camera);
}




Application::~Application()
{
    delete SoundManager::getInstance();
}