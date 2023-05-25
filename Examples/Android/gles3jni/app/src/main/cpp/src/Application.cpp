
#include "Application.h"



Application::Application()  = default;
Application::~Application() = default;


Application::Application(unsigned int screen_width, unsigned int screen_height)
    : SCREEN_WIDTH(screen_width), SCREEN_HEIGHT(screen_height)
{}


void Application::Init(AssetLoader asset_loader)
{
    m_asset_loader = asset_loader;
    ortho_camera = OrthographicCamera(0, (float)SCREEN_WIDTH, 0, (float)SCREEN_HEIGHT);

    rect = Rectangle(0.0f, 1.0f, 1.0f);
    rect.init(m_asset_loader, ortho_camera.getProjectionMatrix());
    rect.translate((float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2);
    rect.scale(100.0f, 100.0f);

    sprite = Sprite();
    sprite.init(m_asset_loader, "smile.jpg", ortho_camera.getProjectionMatrix());
    sprite.translate((float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2);
    sprite.scale(100.0f, 100.0f);
}


void Application::Update(double dt, double time_passed)
{
    // dt: milliseconds passed since last frame
    // time_passed: milliseconds passed since window initialisation
    double delta_time = dt/1000;    // seconds passed since last frame
    float fps = 1000/(float)dt;          // frames per second

    std::cout << fps << " FPS\n";
//    __android_log_print(ANDROID_LOG_ERROR, "APP", "Time passed: %.2f \n", time_passed);

    rect.rotate(-1.10f);
    sprite.rotate(1.10f);
}


void Application::Render(double dt, double time_passed)
{
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, (GLsizei)SCREEN_WIDTH, (GLsizei)SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    rect.draw(ortho_camera);
    sprite.draw(ortho_camera);
}


