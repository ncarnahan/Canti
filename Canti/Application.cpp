#include "Application.h"
#include <iostream>
#include <SDL_image.h>
#include <Graphics/OpenGL.h>

Application::Application() :
    _running(true)
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

    //Create window and context
    _window = SDL_CreateWindow("Title",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_OPENGL);
    _context = SDL_GL_CreateContext(_window);


    //Initialize GLEW
    glewExperimental = true;
    glewInit();
    //GLEW causes an error for some reason
    auto error = glGetError();


    //Initialize SDL_image
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
}

Application::~Application()
{
    IMG_Quit();

    SDL_GL_DeleteContext(_context);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Application::Init()
{
    //Assign the window to the camera
    _camera.SetWindow(_window);
    _camera.SetPosition(Vector3(0, 2, 0));

    _program.LoadFromFiles("Data/Diffuse.vert", "Data/Specular.frag");
    _program.Start();

    _suzanneMesh.LoadObjFile("Data/Suzanne.obj");
    _roomMesh.LoadObjFile("Data/Room.obj");

    TextureLoadSettings textureLoadSettings;
    textureLoadSettings.useSrgbColorSpace = true;
    textureLoadSettings.generateMipmaps = true;
    textureLoadSettings.filter = Graphics::TextureFilter::Trilinear;
    _suzanneTexture.Load("Data/Texture.png", textureLoadSettings);
    _roomTexture.Load("Data/Tiles.png", textureLoadSettings);

    _suzanneMaterial.SetShader(_program);
    _suzanneMaterial.SetTexture(_program.GetUniformLocation("tex_diffuse"), _suzanneTexture);

    _roomMaterial.SetShader(_program);
    _roomMaterial.SetTexture(_program.GetUniformLocation("tex_diffuse"), _roomTexture);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
}

void Application::Update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EventType::SDL_QUIT)
        {
            _running = false;
        }
    }

    Simulate();
    Render();

    SDL_GL_SwapWindow(_window);
}

void Application::Simulate()
{
    _input.Update();
    _camera.Update(0.016f, _input);
}

void Application::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto projection = Matrix4x4::Perspective(65, 16.0f / 9.0f, 0.01f, 1000);
    auto view = _camera.GetViewMatrix();
    auto pv = projection * view;

    glUniformMatrix4fv(_program.GetUniformLocation("in_matrixProj"), 1, false, &projection[0]);
    glUniformMatrix4fv(_program.GetUniformLocation("in_matrixView"), 1, false, &view[0]);

    Matrix4x4 modelMatrix;
    Matrix4x4 pvm;

    Vector3 direction = Quaternion::AngleAxis((float)SDL_GetTicks() / 100.0f, Vector3::up) * Vector3::forward + Vector3(0, 0.2f, 0);
    Vector3 color = Vector3(1, 1, 0.9f);
    glUniform1f(_program.GetUniformLocation("light.intensity"), 1.0f);
    glUniform3fv(_program.GetUniformLocation("light.direction"), 1, &direction[0]);
    glUniform3fv(_program.GetUniformLocation("light.color"), 1, &color[0]);

    Vector3 camPos = _camera.GetPosition();
    glUniform3fv(_program.GetUniformLocation("in_eyePosition"), 1, &camPos[0]);

    modelMatrix = Matrix4x4::FromPosition(Vector3(0, -2, 0));
    pvm = pv * modelMatrix;
    glUniformMatrix4fv(_program.GetUniformLocation("in_matrixModel"), 1, false, &modelMatrix[0]);
    glUniformMatrix4fv(_program.GetUniformLocation("in_matrixPVM"), 1, false, &pvm[0]);
    _roomMaterial.Start();
    _roomMesh.Draw();

    modelMatrix = Matrix4x4::FromTransform(Vector3(0, 0, -4), Quaternion::AngleAxis((float)SDL_GetTicks() / 10.f, Vector3::left), Vector3::one);
    pvm = pv * modelMatrix;
    glUniformMatrix4fv(_program.GetUniformLocation("in_matrixModel"), 1, false, &modelMatrix[0]);
    glUniformMatrix4fv(_program.GetUniformLocation("in_matrixPVM"), 1, false, &pvm[0]);
    _suzanneMaterial.Start();
    _suzanneMesh.Draw();

    modelMatrix = Matrix4x4::FromPosition(Vector3(6, 0, 0));
    pvm = pv * modelMatrix;
    glUniformMatrix4fv(_program.GetUniformLocation("in_matrixModel"), 1, false, &modelMatrix[0]);
    glUniformMatrix4fv(_program.GetUniformLocation("in_matrixPVM"), 1, false, &pvm[0]);
    _suzanneMesh.Draw();
    
    auto error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << error << " : " << glewGetErrorString(error);
    }
}
