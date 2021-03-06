#ifndef CANTI_APPLICATION_H
#define CANTI_APPLICATION_H

#include <SDL.h>
#include <Graphics/Renderer.h>
#include <Graphics/Program.h>
#include <Graphics/Texture.h>
#include <Graphics/StaticMesh.h>
#include <Graphics/Light.h>
#include <Graphics/Material.h>
#include <Graphics/ShadowMap.h>
using namespace Graphics;
#include <Utils/Input.h>
using namespace Utils;
#include "FpsCamera.h"
#include "Entity.h"
#include "GBuffer.h"

class Application
{
private:
    bool _running;
    SDL_Window* _window;
    SDL_GLContext _context;

    bool _updateScene;
    bool _useDeferredShading;

    GBuffer _gbuffer;
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

    Material _tangentMaterial;

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

    Program _depthProgram;
    Material _depthMaterial;
    
    StaticMesh _quad;
    Program _deferredLightProgram;
    Material _deferredLightMaterial;
    Program _deferredDiffuseProgram;
    Program _deferredSpecularProgram;
    Program _deferredBumpedDiffuseProgram;
    Program _deferredBumpedSpecularProgram;
    Material _deferredTileMaterial1;
    Material _deferredTileMaterial2;
    Material _deferredTileMaterial3;
    Material _deferredTileMaterial4;

    std::vector<Entity> _entities;
    std::vector<Light> _lights;
    std::vector<ShadowMap> _shadowMaps;
    std::vector<Texture> _shadowMapTextures;


public:
    Application();
    ~Application();

    void Init();
    void Update();

    inline bool IsRunning() { return _running; }

private:
    void Simulate();
    void RenderShadowMaps();
    void RenderDeferred();
    void RenderForward();
};

#endif
