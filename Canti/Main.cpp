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
#include <Utils/Input.h>
using namespace Utils;


void Init();
void Update();
void Display();


Input input;
Mesh mesh;
Shader shader;
Texture texture;
Material material;

Vector3 cameraPos = Vector3(0, 2, 0);
Vector2 cameraRot;


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
        1280, 720, SDL_WINDOW_OPENGL);
    
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

        Update();
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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Update()
{
    input.Update();

    const float sensitivity = 0.2f;
    cameraRot.x -= sensitivity * input.GetRelativeMouseX();
    cameraRot.y = Math::Clamp(cameraRot.y - sensitivity * input.GetRelativeMouseY(), -90.0f, 90.0f);


    Quaternion rotation = Quaternion::AngleAxis(cameraRot.x, Vector3::up) * Quaternion::AngleAxis(cameraRot.y, Vector3::right);

    //Forward and back
    if (input.KeyDown(SDL_SCANCODE_W))
    {
        cameraPos += 0.1f * (rotation * Vector3::forward);
    }
    else if (input.KeyDown(SDL_SCANCODE_S))
    {
        cameraPos += 0.1f * (rotation * Vector3::back);
    }

    //Left and right
    if (input.KeyDown(SDL_SCANCODE_A))
    {
        cameraPos += 0.1f * (rotation * Vector3::left);
    }
    else if (input.KeyDown(SDL_SCANCODE_D))
    {
        cameraPos += 0.1f * (rotation * Vector3::right);
    }

    //Up and down
    if (input.KeyDown(SDL_SCANCODE_Q))
    {
        cameraPos += 0.1f * (rotation * Vector3::down);
    }
    else if (input.KeyDown(SDL_SCANCODE_E))
    {
        cameraPos += 0.1f * (rotation * Vector3::up);
    }
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto projection = Matrix4x4::Perspective(60, 16.0f / 9.0f, 0.01f, 1000);
    auto view = Matrix4x4::FromTransform(
        cameraPos,
        Quaternion::AngleAxis(cameraRot.x, Vector3::up) * Quaternion::AngleAxis(cameraRot.y, Vector3::right),
        Vector3::one).GetInverse();
    auto matrix = projection * view * Matrix4x4::FromPosition(Vector3(0, 0, -4));
    glUniformMatrix4fv(shader.GetUniformLocation("in_modelview"), 1, false, &matrix[0]);

    material.Start();
    mesh.Draw();
    
    auto error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << error << " : " << glewGetErrorString(error);
    }
}
