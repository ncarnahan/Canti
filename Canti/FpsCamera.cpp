#include "FpsCamera.h"

void FpsCamera::Update(float dt, Input& input)
{
    const float sensitivity = 0.2f;
    _rotation.x -= sensitivity * input.GetRelativeMouseX();
    _rotation.y = Math::Clamp(_rotation.y - sensitivity * input.GetRelativeMouseY(), -90.0f, 90.0f);

    const float speed = 8.0f;
    float distance = speed * dt;

    Quaternion rotation = GetRotation();

    //Forward and back
    if (input.KeyDown(SDL_SCANCODE_W))
    {
        _position += distance * (rotation * Vector3::forward);
    }
    else if (input.KeyDown(SDL_SCANCODE_S))
    {
        _position += distance * (rotation * Vector3::back);
    }

    //Left and right
    if (input.KeyDown(SDL_SCANCODE_A))
    {
        _position += distance * (rotation * Vector3::left);
    }
    else if (input.KeyDown(SDL_SCANCODE_D))
    {
        _position += distance * (rotation * Vector3::right);
    }

    //Up and down
    if (input.KeyDown(SDL_SCANCODE_Q))
    {
        _position += distance * (rotation * Vector3::down);
    }
    else if (input.KeyDown(SDL_SCANCODE_E))
    {
        _position += distance * (rotation * Vector3::up);
    }
}
