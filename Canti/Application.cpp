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
    _cutoutProgram.LoadFromFiles("Data/CutoutDiffuse.vert", "Data/CutoutDiffuse.frag");
    _additiveProgram.LoadFromFiles("Data/Additive.vert", "Data/Additive.frag");
    _tangentProgram.LoadFromFiles("Data/TangentVisualization.vert", "Data/TangentVisualization.geom", "Data/TangentVisualization.frag");
    
    _roomMesh.LoadObjFile("Data/Room.obj");
    _cubeMesh.LoadObjFile("Data/Cube.obj");
    _suzanneMesh.LoadObjFile("Data/Suzanne.obj");

    TextureLoadSettings diffuseSettings;
    diffuseSettings.useSrgbColorSpace = true;
    diffuseSettings.generateMipmaps = true;
    diffuseSettings.filter = Graphics::TextureFilter::Trilinear;
    _suzanneTexture.Load("Data/Texture.png", diffuseSettings);
    _tileTexture.Load("Data/Tiles.png", diffuseSettings);
    _blendedTexture.Load("Data/Blended.png", diffuseSettings);
    _particleTexture.Load("Data/Particle.png", diffuseSettings);

    TextureLoadSettings normalSettings;
    normalSettings.useSrgbColorSpace = false;
    normalSettings.generateMipmaps = true;
    normalSettings.filter = Graphics::TextureFilter::Trilinear;
    _tileNormalTexture.Load("Data/TilesNormal.png", normalSettings);

    _suzanneMaterial.SetProgram(_diffuseProgram);
    _suzanneMaterial.SetTexture("tex_diffuse", _suzanneTexture);

    _tileMaterial1.SetProgram(_diffuseProgram);
    _tileMaterial1.SetTexture("tex_diffuse", _tileTexture);

    _tileMaterial2.SetProgram(_specularProgram);
    _tileMaterial2.SetTexture("tex_diffuse", _tileTexture);
    _tileMaterial2.SetVector3("material.specularColor", Vector3(1, 0, 0));
    _tileMaterial2.SetFloat("material.specularExponent", 100);

    _tileMaterial3.SetProgram(_bumpedDiffuseProgram);
    _tileMaterial3.SetTexture("tex_diffuse", _tileTexture);
    _tileMaterial3.SetTexture("tex_normal", _tileNormalTexture);

    _tileMaterial4.SetProgram(_bumpedSpecularProgram);
    _tileMaterial4.SetTexture("tex_diffuse", _tileTexture);
    _tileMaterial4.SetTexture("tex_normal", _tileNormalTexture);
    _tileMaterial4.SetVector3("material.specularColor", Vector3(1, 0, 0));
    _tileMaterial4.SetFloat("material.specularExponent", 100);

    _blendedMaterial.SetProgram(_diffuseProgram);
    _blendedMaterial.SetBlendType(BlendType::Transparent);
    _blendedMaterial.SetTexture("tex_diffuse", _blendedTexture);

    _cutoutMaterial.SetProgram(_cutoutProgram);
    _cutoutMaterial.SetBlendType(BlendType::AlphaTested);
    _cutoutMaterial.SetTexture("tex_diffuse", _blendedTexture);
    _cutoutMaterial.SetFloat("material.cutoff", 0.5f);

    _particleMaterial.SetProgram(_additiveProgram);
    _particleMaterial.SetBlendType(BlendType::Additive);
    _particleMaterial.SetTexture("tex_diffuse", _particleTexture);

    
    {
        Entity entity;
        entity.mesh = &_roomMesh;
        entity.material = &_tileMaterial4;
        entity.position = Vector3(0, -2, -8);
        entity.scale = 2;
        _entities.push_back(entity);
    }

    //Suzanne
    {
        Entity entity;
        entity.mesh = &_suzanneMesh;
        entity.material = &_suzanneMaterial;
        entity.position = Vector3(0, 0, -14);
        entity.scale = 2;
        _entities.push_back(entity);
    }

    //Four solid cubes
    {
        Entity entity;
        entity.mesh = &_cubeMesh;
        entity.material = &_tileMaterial1;
        entity.position = Vector3(-6, 0, -2);
        _entities.push_back(entity);
    }

    {
        Entity entity;
        entity.mesh = &_cubeMesh;
        entity.material = &_tileMaterial2;
        entity.position = Vector3(-2, 0, -2);
        _entities.push_back(entity);
    }

    {
        Entity entity;
        entity.mesh = &_cubeMesh;
        entity.material = &_tileMaterial3;
        entity.position = Vector3(2, 0, -2);
        _entities.push_back(entity);
    }

    {
        Entity entity;
        entity.mesh = &_cubeMesh;
        entity.material = &_tileMaterial4;
        entity.position = Vector3(6, 0, -2);
        _entities.push_back(entity);
    }


    //Four blended cubes
    {
        Entity entity;
        entity.mesh = &_cubeMesh;
        entity.material = &_blendedMaterial;
        entity.position = Vector3(-6, 0, -10);
        _entities.push_back(entity);
    }

    {
        Entity entity;
        entity.mesh = &_cubeMesh;
        entity.material = &_blendedMaterial;
        entity.position = Vector3(-2, 0, -10);
        _entities.push_back(entity);
    }

    {
        Entity entity;
        entity.mesh = &_cubeMesh;
        entity.material = &_blendedMaterial;
        entity.position = Vector3(2, 0, -10);
        _entities.push_back(entity);
    }

    {
        Entity entity;
        entity.mesh = &_cubeMesh;
        entity.material = &_blendedMaterial;
        entity.position = Vector3(6, 0, -10);
        _entities.push_back(entity);
    }


    //Four different transparency cubes
    {
        Entity entity;
        entity.mesh = &_cubeMesh;
        entity.material = &_tileMaterial1;
        entity.position = Vector3(-6, 0, -6);
        _entities.push_back(entity);
    }

    {
        Entity entity;
        entity.mesh = &_cubeMesh;
        entity.material = &_cutoutMaterial;
        entity.position = Vector3(-2, 0, -6);
        _entities.push_back(entity);
    }

    {
        Entity entity;
        entity.mesh = &_cubeMesh;
        entity.material = &_blendedMaterial;
        entity.position = Vector3(2, 0, -6);
        _entities.push_back(entity);
    }

    {
        Entity entity;
        entity.mesh = &_cubeMesh;
        entity.material = &_particleMaterial;
        entity.position = Vector3(6, 0, -6);
        _entities.push_back(entity);
    }


    {
        Light light;
        light.Directional(Vector3(0.5f, 1, 1.5f), Vector3::one, 0.02f);
        _lights.push_back(light);
    }

    {
        Light light;
        light.Point(Vector3(0, 2, -6), Vector3(1, 1, 1), 2.0f, 8.0f);
        _lights.push_back(light);
    }

    {
        Light light;
        light.Spot(Vector3(0, 2, -6), Vector3::forward, Vector3(0.9f, 0.9f, 1), 5, 12, 30, 0.0f);
        _lights.push_back(light);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.05f, 0.05f, 0.05f, 0.0f);
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
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto projection = Matrix4x4::Perspective(65, 16.0f / 9.0f, 0.01f, 1000);
    auto view = _camera.GetViewMatrix();
    auto pv = projection * view;

    Vector3 camPos = _camera.GetPosition();

    Vector3 ambient(0.1f, 0.1f, 0.1f);
    Vector3 zero;

    Matrix4x4 modelMatrix;
    Matrix4x4 pvm;


    _lights[2].direction = Quaternion::AngleAxis((float)SDL_GetTicks() * 0.1f, Vector3::up) * Vector3(1, -1, 0);


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

        
        //Ambient lighting on first pass
        glUniform3fv(program->GetUniformLocation("light.ambient"), 1, &ambient[0]);

        if (entity.material->IsLit())
        {
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


                entity.material->SecondPass();

                //Disable ambient for future passes
                glUniform3fv(program->GetUniformLocation("light.ambient"), 1, &zero[0]);
            }
        }
        else
        {
            //Draw
            entity.mesh->Draw();
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
