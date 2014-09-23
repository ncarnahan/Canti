#ifndef CANTI_APPLICATION_H
#define CANTI_APPLICATION_H

#include <SDL.h>
#include <Graphics/Shader.h>
#include <Graphics/Texture.h>
#include <Graphics/StaticMesh.h>
#include <Graphics/Material.h>
using namespace Graphics;
#include <Utils/Input.h>
using namespace Utils;
#include "FpsCamera.h"

class Application
{
private:
    bool _running;
    SDL_Window* _window;
    SDL_GLContext _context;

    Input _input;
    FpsCamera _camera;
    StaticMesh _mesh;
    Shader _shader;
    Texture _texture;
    Material _material;


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
