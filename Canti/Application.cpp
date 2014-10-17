#include "Application.h"
#include <iostream>
#include <SDL_image.h>
#include <Graphics/OpenGL.h>

Application::Application() :
    _running(true),
    _showTangents(false)
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

    _diffuseProgram.LoadFromFiles("Data/Diffuse.vert", "Data/Diffuse.frag");
    _specularProgram.LoadFromFiles("Data/Specular.vert", "Data/Specular.frag");
    _bumpedDiffuseProgram.LoadFromFiles("Data/BumpedDiffuse.vert", "Data/BumpedDiffuse.frag");
    _bumpedSpecularProgram.LoadFromFiles("Data/BumpedSpecular.vert", "Data/BumpedSpecular.frag");
    _tangentProgram.LoadFromFiles("Data/TangentVisualization.vert", "Data/TangentVisualization.geom", "Data/TangentVisualization.frag");

    _suzanneMesh.LoadObjFile("Data/Suzanne.obj");
    _roomMesh.LoadObjFile("Data/Room.obj");
    _cyllinderMesh.LoadObjFile("Data/NormalMapTest.obj");

    TextureLoadSettings diffuseSettings;
    diffuseSettings.useSrgbColorSpace = true;
    diffuseSettings.generateMipmaps = true;
    diffuseSettings.filter = Graphics::TextureFilter::Trilinear;
    _suzanneTexture.Load("Data/Texture.png", diffuseSettings);
    _roomTexture.Load("Data/Tiles.png", diffuseSettings);

    TextureLoadSettings normalSettings;
    normalSettings.useSrgbColorSpace = false;
    normalSettings.generateMipmaps = true;
    normalSettings.filter = Graphics::TextureFilter::Trilinear;
    _cyllinderNormalTexture.Load("Data/NormalMapTest.png", normalSettings);
    _roomNormalTexture.Load("Data/TilesNormal.png", normalSettings);

    _suzanneMaterial.SetProgram(_diffuseProgram);
    _suzanneMaterial.SetTexture("tex_diffuse", _suzanneTexture);

    _roomMaterial.SetProgram(_bumpedSpecularProgram);
    _roomMaterial.SetTexture("tex_diffuse", _roomTexture);
    _roomMaterial.SetTexture("tex_normal", _roomNormalTexture);
    _roomMaterial.SetVector3("material.specularColor", Vector3(1, 0, 0));
    _roomMaterial.SetFloat("material.specularExponent", 100);

    _cyllinderMaterial.SetProgram(_bumpedSpecularProgram);
    _cyllinderMaterial.SetTexture("tex_diffuse", _roomTexture);
    _cyllinderMaterial.SetTexture("tex_normal", _cyllinderNormalTexture);
    _cyllinderMaterial.SetVector3("material.specularColor", Vector3(0, 0, 1));
    _cyllinderMaterial.SetFloat("material.specularExponent", 64);

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
        entity.scale = 0.05f;
        _entities.push_back(entity);
    }

    {
        Entity entity;
        entity.mesh = &_cyllinderMesh;
        entity.material = &_cyllinderMaterial;
        entity.position = Vector3(0, -1, 2);
        _entities.push_back(entity);
    }

    {
        Light light;
        light.Point(Vector3::zero, Vector3(1, 0, 0), 1.0f, 5.0f);
        _lights.push_back(light);
    }

    {
        Light light;
        light.Directional(Vector3(0.5f, 1, 1.5f), Vector3::one, 0.02f);
        _lights.push_back(light);
    }

    {
        Light light;
        light.Point(Vector3(0, 0, -6), Vector3(1, 1, 1), 2.0f, 8.0f);
        _lights.push_back(light);
    }

    {
        Light light;
        light.Spot(Vector3(4, 0, 0), Vector3::back, Vector3(0, 1, 0), 5, 12, 30, 0.0f);
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

    if (_input.KeyPressed(SDL_SCANCODE_T)) { _showTangents = !_showTangents; }

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

    Vector3 camPos = _camera.GetPosition();

    _entities[1].rotation = Quaternion::AngleAxis((float)SDL_GetTicks() / 10.0f, Vector3::left);
    _entities[1].scale = Math::SinDeg((float)SDL_GetTicks() / 11.0f) / 2 + 1;

    if (_input.KeyDown(SDL_SCANCODE_I)) { _lights[0].position.z += 0.05f; }
    if (_input.KeyDown(SDL_SCANCODE_K)) { _lights[0].position.z -= 0.05f; }
    if (_input.KeyDown(SDL_SCANCODE_U)) { _lights[0].position.y += 0.05f; }
    if (_input.KeyDown(SDL_SCANCODE_O)) { _lights[0].position.y -= 0.05f; }
    if (_input.KeyDown(SDL_SCANCODE_J)) { _lights[0].position.x += 0.05f; }
    if (_input.KeyDown(SDL_SCANCODE_L)) { _lights[0].position.x -= 0.05f; }
    _entities[2].position = _lights[0].position;
    
    _lights[3].direction = Quaternion::AngleAxis((float)SDL_GetTicks() / 50.0f, Vector3::up) * Vector3::forward;
    //_lights[3].innerPercent = (Math::SinDeg((float)SDL_GetTicks() / 11.0f) + 1.0f) / 2.0f;
    
    Vector3 ambient(0.1f, 0.1f, 0.1f);
    Vector3 zero;

    Matrix4x4 modelMatrix;
    Matrix4x4 pvm;

    for (auto& entity : _entities)
    {
        //Use the material
        entity.material->Start();

        auto program = entity.material->GetProgram();

        //TEMP: Use UBO
        glUniformMatrix4fv(program->GetUniformLocation("in_matrixProj"), 1, false, &projection[0]);
        glUniformMatrix4fv(program->GetUniformLocation("in_matrixView"), 1, false, &view[0]);
        glUniform3fv(program->GetUniformLocation("in_eyePosition"), 1, &camPos[0]);

        //Model matrix
        modelMatrix = Matrix4x4::FromTransform(
            entity.position, entity.rotation, Vector3(entity.scale));

        pvm = pv * modelMatrix;

        //Matrices
        glUniformMatrix4fv(program->GetUniformLocation("in_matrixModel"), 1, false, &modelMatrix[0]);
        glUniformMatrix4fv(program->GetUniformLocation("in_matrixPVM"), 1, false, &pvm[0]);


        //Disable blending; we're drawing the first one
        glDisable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glDepthFunc(GL_LESS);

        
        //Ambient lighting on first pass
        glUniform3fv(program->GetUniformLocation("light.ambient"), 1, &ambient[0]);

        for (auto& light : _lights)
        {
            //Light uniforms
            glUniform1i(program->GetUniformLocation("light.type"), (int)light.type);

            glUniform3fv(program->GetUniformLocation("light.position"), 1, &light.position[0]);
            glUniform3fv(program->GetUniformLocation("light.direction"), 1, &light.direction[0]);
            glUniform3fv(program->GetUniformLocation("light.color"), 1, &light.color[0]);
            
            glUniform1f(program->GetUniformLocation("light.intensity"), light.intensity);
            glUniform1f(program->GetUniformLocation("light.radius"), light.radius);
            glUniform1f(program->GetUniformLocation("light.cosAngle"), Math::CosDeg(light.angle));
            glUniform1f(program->GetUniformLocation("light.innerPercent"), light.innerPercent);
            

            //Draw
            entity.mesh->Draw();


            //Additive lighting; enable blending and allow equal depth
            glEnable(GL_BLEND);
            glDepthFunc(GL_LEQUAL);

            //Disable ambient for future passes
            glUniform3fv(program->GetUniformLocation("light.ambient"), 1, &zero[0]);
        }

        //Tangent Visualization
        if (_showTangents)
        {
            program = &_tangentProgram;
            program->Start();
            glUniformMatrix4fv(program->GetUniformLocation("in_matrixProj"), 1, false, &projection[0]);
            glUniformMatrix4fv(program->GetUniformLocation("in_matrixView"), 1, false, &view[0]);
            glUniformMatrix4fv(program->GetUniformLocation("in_matrixModel"), 1, false, &modelMatrix[0]);
            glUniformMatrix4fv(program->GetUniformLocation("in_matrixPVM"), 1, false, &pvm[0]);
            entity.mesh->DrawPoints();
        }
    }
    
    auto error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << error << " : " << glewGetErrorString(error);
    }
}
