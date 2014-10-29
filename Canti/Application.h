#ifndef CANTI_APPLICATION_H
#define CANTI_APPLICATION_H

#include <SDL.h>
#include <Graphics/Renderer.h>
#include <Graphics/Program.h>
#include <Graphics/Texture.h>
#include <Graphics/StaticMesh.h>
#include <Graphics/Light.h>
#include <Graphics/Material.h>
using namespace Graphics;
#include <Utils/Input.h>
using namespace Utils;
#include "FpsCamera.h"
#include "Entity.h"

class Application
{
private:
    bool _running;
    SDL_Window* _window;
    SDL_GLContext _context;

    Renderer _renderer;
    Input _input;
    FpsCamera _camera;

    Program _diffuseProgram;
    Program _specularProgram;
    Program _bumpedDiffuseProgram;
    Program _bumpedSpecularProgram;
    Program _cutoutProgram;
    Program _additiveProgram;
    Program _tangentProgram;
    bool _showTangents;

    StaticMesh _roomMesh;
    StaticMesh _cubeMesh;

    StaticMesh _suzanneMesh;
    Texture _suzanneTexture;
    Material _suzanneMaterial;
    
    Texture _tileTexture;
    Texture _tileNormalTexture;
    Material _tileMaterial1;
    Material _tileMaterial2;
    Material _tileMaterial3;
    Material _tileMaterial4;

    Texture _blendedTexture;
    Material _blendedMaterial;
    Material _cutoutMaterial;

    Texture _particleTexture;
    Material _particleMaterial;

    std::vector<Entity> _entities;
    std::vector<Light> _lights;


public:
    Application();
    ~Application();

    void Init();
    void Update();

    inline bool IsRunning() { return _running; }

private:
    void Simulate();
    void Render();
};

#endif
