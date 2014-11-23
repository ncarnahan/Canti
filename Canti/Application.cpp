#include "Application.h"
#include <iostream>
#include <SDL_image.h>
#include <Graphics/OpenGL.h>

Application::Application() :
    _running(true),
    _updateScene(true),
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

    _tangentMaterial.SetProgram(_tangentProgram);
    _tangentMaterial.useLighting = false;

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
    _blendedMaterial.blendType = BlendType::Transparent;
    _blendedMaterial.SetTexture("tex_diffuse", _blendedTexture);

    _cutoutMaterial.SetProgram(_cutoutProgram);
    _cutoutMaterial.blendType = BlendType::AlphaTested;
    _cutoutMaterial.SetTexture("tex_diffuse", _blendedTexture);
    _cutoutMaterial.SetFloat("material.cutoff", 0.5f);

    _particleMaterial.SetProgram(_additiveProgram);
    _particleMaterial.blendType = BlendType::Additive;
    _particleMaterial.useLighting = false;
    _particleMaterial.SetTexture("tex_diffuse", _particleTexture);
    

    
    //Shadow casting stuff
    _depthProgram.LoadFromFiles("Data/Depth.vert", "Data/Depth.frag");
    _depthMaterial.SetProgram(_depthProgram);
    _depthMaterial.useLighting = false;

    TextureLoadSettings depthMapSettings;
    depthMapSettings.clamp = TextureClamp::Clamp;
    depthMapSettings.filter = TextureFilter::Nearest;
    _directionalShadowTexture.Create(1024, 1024,
        TextureFormat::Depth24, depthMapSettings);

    _directionalShadowMap.framebuffer.CreateDepth(_directionalShadowTexture);
    _directionalShadowMap.bias = 0.02f;
    _directionalShadowMap.strength = 0.5f;


    {
        Entity entity;
        entity.mesh = &_roomMesh;
        entity.material = &_tileMaterial1;
        entity.position = Vector3(0, 0, 0);
        _entities.push_back(entity);
    }

    {
        Entity entity;
        entity.mesh = &_cubeMesh;
        entity.material = &_tileMaterial1;
        entity.position = Vector3(0, 4, 0);
        _entities.push_back(entity);
    }


    //Cache a sortkey for each entity
    for (Entity& entity : _entities)
    {
        entity.sortKey = _renderer.CreateSortKey(0, entity.material, 0);
    }


    {
        Light light;
        light.Directional(Vector3(0, 1, 0), Vector3(1), 2);
        light.position = Vector3(0, 10, 0);
        light.shadowMap = &_directionalShadowMap;
        _lights.push_back(light);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    float v = pow(0.05f, 2.2f);
    glClearColor(v, v, v, 0.0f);
    glEnable(GL_FRAMEBUFFER_SRGB);
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

    _input.Update();

    if (_input.KeyPressed(SDL_SCANCODE_SPACE)) { _updateScene = !_updateScene; }
    if (_input.KeyPressed(SDL_SCANCODE_T)) { _showTangents = !_showTangents; }
    if (_input.KeyPressed(SDL_SCANCODE_R)) { _renderer.sortEnabled = !_renderer.sortEnabled; }
    if (_input.KeyPressed(SDL_SCANCODE_MINUS)) { _renderer.ignoreCount++; }
    if (_input.KeyPressed(SDL_SCANCODE_EQUALS)) { _renderer.ignoreCount = Math::Max(_renderer.ignoreCount - 1, 0); }
    std::cout << "Ignoring: " << _renderer.ignoreCount << std::endl;

    Simulate();
    RenderShadowMaps();
    Render();

    SDL_GL_SwapWindow(_window);
}

void Application::Simulate()
{
    _camera.Update(0.016f, _input);

    _entities[1].position = Vector3(
        5 * sin((float)SDL_GetTicks() / 400.0f), 5,
        5 * cos((float)SDL_GetTicks() / 400.0f));
}

void Application::RenderShadowMaps()
{
    _directionalShadowMap.framebuffer.Start();

    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

    Light& light = _lights[0];
    light.position = Vector3(0, 10, 0);

    Matrix4x4 projMatrix = Matrix4x4::Orthographic(-10, 10, -10, 10, 0, 40);
    _renderer.SetProjectionMatrix(projMatrix);
    Matrix4x4 viewMatrix = Matrix4x4::FromTransform(
        light.position,
        Quaternion::AngleAxis(-80, Vector3::right) * Quaternion::AngleAxis(30, Vector3::up),
        Vector3(1)).GetInverse();
    _renderer.SetViewMatrix(viewMatrix);

    _directionalShadowMap.matrixPV = projMatrix * viewMatrix;

    for (auto& entity : _entities)
    {
        //Update sort key depth
        entity.sortKey.UpdateDepth(
            Vector3::DistanceSqr(_camera.GetPosition(), entity.position));
        entity.sortKey.UpdatePass(0);

        DrawCall drawCall;
        entity.mesh->FillDrawCall(drawCall);

        drawCall.pass = 0;
        drawCall.material = &_depthMaterial;
        drawCall.modelMatrix = Matrix4x4::FromTransform(
            entity.position, entity.rotation, Vector3(entity.scale));

        _renderer.Submit(entity.sortKey, drawCall);
    }

    _renderer.Draw();

    _directionalShadowMap.framebuffer.Stop();
}

void Application::Render()
{
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    
    _renderer.SetProjectionMatrix(Matrix4x4::Perspective(65, 16.0f / 9.0f, 0.01f, 1000));
    _renderer.SetViewMatrix(_camera.GetViewMatrix());
    _renderer.SetEyePosition(_camera.GetPosition());

    const Vector3 ambient(pow(0.1f, 1 / 2.2f));


    uint8_t pass;
    Vector3 drawCallAmbient;

    for (auto& entity : _entities)
    {
        pass = 0;
        drawCallAmbient = ambient;

        //Update sort key depth
        entity.sortKey.UpdateDepth(
            Vector3::DistanceSqr(_camera.GetPosition(), entity.position));

        if (entity.material->useLighting)
        {
            for (auto& light : _lights)
            {
                DrawCall drawCall;
                entity.mesh->FillDrawCall(drawCall);

                drawCall.pass = pass;
                drawCall.material = entity.material;
                drawCall.modelMatrix = Matrix4x4::FromTransform(
                    entity.position, entity.rotation, Vector3(entity.scale));
                drawCall.ambientLight = drawCallAmbient;
                drawCall.light = &light;

                entity.sortKey.UpdatePass(pass);

                _renderer.Submit(entity.sortKey, drawCall);

                //Disable ambient for future passes
                drawCallAmbient = Vector3(0);
                pass = 1;
            }
        }
        else
        {
            DrawCall drawCall;
            entity.mesh->FillDrawCall(drawCall);

            drawCall.pass = pass;
            drawCall.material = entity.material;
            drawCall.modelMatrix = Matrix4x4::FromTransform(
                entity.position, entity.rotation, Vector3(entity.scale));

            entity.sortKey.UpdatePass(pass);

            _renderer.Submit(entity.sortKey, drawCall);
        }
        

        //Tangent Visualization
        if (_showTangents)
        {
            DrawCall drawCall;
            entity.mesh->FillDrawCall(drawCall);

            drawCall.pass = 2;
            drawCall.material = &_tangentMaterial;
            drawCall.drawMode = GL_POINTS;
            drawCall.modelMatrix = Matrix4x4::FromTransform(
                entity.position, entity.rotation, Vector3(entity.scale));

            entity.sortKey.UpdatePass(2);

            _renderer.Submit(entity.sortKey, drawCall);
        }
    }

    _renderer.Draw();
    
    auto error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << error << " : " << glewGetErrorString(error);
    }
}
