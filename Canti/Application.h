#ifndef CANTI_APPLICATION_H
#define CANTI_APPLICATION_H

#include <SDL.h>
#include <Graphics/Program.h>
#include <Graphics/Texture.h>
#include <Graphics/StaticMesh.h>
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

    Input _input;
    FpsCamera _camera;

    Program _program;

    StaticMesh _suzanneMesh;
    Texture _suzanneTexture;
    Material _suzanneMaterial;
    
    StaticMesh _roomMesh;
    Texture _roomTexture;
    Material _roomMaterial;

    std::vector<Entity> _entities;


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
