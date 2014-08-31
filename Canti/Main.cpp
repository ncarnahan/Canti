#include "Common.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <Graphics/OpenGL.h>
#include <Graphics/Shader.h>
#include <Graphics/Texture.h>
#include <Graphics/Mesh.h>
#include <Graphics/Material.h>
using namespace Graphics;


void Init();
void Display();


Mesh mesh;
Shader shader;
Texture texture;
Material material;


int main(int argc, char **argv)
{
    bool running = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

    SDL_Window* window = SDL_CreateWindow("Title",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewExperimental = true;
    glewInit();

    //GLEW causes an error for some reason
    auto error = glGetError();

    std::cout << "OpenGL Renderer: " << (char*)glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << (char*)glGetString(GL_VERSION) << std::endl;

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    Init();

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EventType::SDL_QUIT)
            {
                running = false;
            }
        }

        Display();

        SDL_GL_SwapWindow(window);
    }

    IMG_Quit();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void Init()
{
    shader.LoadFromFiles("Data/Diffuse.vert", "Data/Diffuse.frag");
    shader.Start();

    mesh.LoadObjFile("Data/Hallway.obj");

    texture.Load("Data/Hallway.png");

    material.SetShader(shader);
    material.SetTexture(shader.GetUniformLocation("tex_diffuse"), texture);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto projection = Matrix4x4::Perspective(60, 4.0f / 3.0f, 0.01f, 1000);
    auto matrix = Matrix4x4::FromTransform(
        Vector3(0, -2, 0),
        Quaternion::AngleAxis(SDL_GetTicks() / 20, Vector3::up),
        Vector3::one);

    matrix = projection * matrix;
    glUniformMatrix4fv(shader.GetUniformLocation("in_modelview"), 1, false, &matrix[0]);

    material.Start();
    mesh.Draw();
    
    auto error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << error << " : " << glewGetErrorString(error);
    }
}