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

    {
        Entity entity;
        entity.mesh = &_roomMesh;
        entity.material = &_roomMaterial;
        entity.position = Vector3(0, -2, 0);
        _entities.push_back(entity);
    }

    {
        Entity entity;
        entity.mesh = &_suzanneMesh;
        entity.material = &_suzanneMaterial;
        entity.position = Vector3(0, 0, -4);
        _entities.push_back(entity);
    }

    {
        Entity entity;
        entity.mesh = &_suzanneMesh;
        entity.material = &_suzanneMaterial;
        entity.position = Vector3(6, 0, 0);
        _entities.push_back(entity);
    }

    {
        Light light;
        light.Directional(Vector3(1, 1, 1), Vector3::one, 0.02f);
        _lights.push_back(light);
    }

    {
        Light light;
        light.Point(Vector3::zero, Vector3(1, 0, 0), 1.0f, 5.0f);
        _lights.push_back(light);
    }

    {
        Light light;
        light.Point(Vector3(0, 0, -6), Vector3(0, 0, 1), 8.0f, 8.0f);
        _lights.push_back(light);
    }

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

    Vector3 camPos = _camera.GetPosition();
    glUniform3fv(_program.GetUniformLocation("in_eyePosition"), 1, &camPos[0]);

    _entities[1].rotation = Quaternion::AngleAxis((float)SDL_GetTicks() / 10.f, Vector3::left);
    


    Matrix4x4 modelMatrix;
    Matrix4x4 pvm;

    for (auto& entity : _entities)
    {
        //Model matrix
        modelMatrix = Matrix4x4::FromTransform(
            entity.position, entity.rotation, Vector3(entity.scale));

        pvm = pv * modelMatrix;

        //Matrices
        glUniformMatrix4fv(_program.GetUniformLocation("in_matrixModel"), 1, false, &modelMatrix[0]);
        glUniformMatrix4fv(_program.GetUniformLocation("in_matrixPVM"), 1, false, &pvm[0]);


        //Disable blending; we're drawing the first one
        glDisable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glDepthFunc(GL_LESS);


        for (auto& light : _lights)
        {
            //Light uniforms
            glUniform1i(_program.GetUniformLocation("light.type"), (int)light.GetType());

            glUniform3fv(_program.GetUniformLocation("light.position"), 1, &light.GetPosition()[0]);
            glUniform3fv(_program.GetUniformLocation("light.direction"), 1, &light.GetDirection()[0]);
            glUniform3fv(_program.GetUniformLocation("light.color"), 1, &light.GetColor()[0]);
            
            glUniform1f(_program.GetUniformLocation("light.intensity"), light.GetIntensity());
            glUniform1f(_program.GetUniformLocation("light.radius"), light.GetRadius());


            //Draw
            entity.material->Start();
            entity.mesh->Draw();


            //Additive lighting; enable blending and allow equal depth
            glEnable(GL_BLEND);
            glDepthFunc(GL_LEQUAL);
        }
    }
    
    auto error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << error << " : " << glewGetErrorString(error);
    }
}
