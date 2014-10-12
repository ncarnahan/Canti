#include "FpsCamera.h"

void FpsCamera::Update(float dt, Input& input)
{
    if (input.ButtonPressed(2))
    {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_GetMouseState(&_mouseDownX, &_mouseDownY);
    }
    else if (input.ButtonReleased(2))
    {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        SDL_WarpMouseInWindow(_window, _mouseDownX, _mouseDownY);
    }

    if (input.ButtonDown(2))
    {
        const float sensitivity = 0.2f;
        _rotation.x -= sensitivity * input.GetRelativeMouseX();
        _rotation.y = Math::Clamp(_rotation.y - sensitivity * input.GetRelativeMouseY(), -90.0f, 90.0f);

        float speed = 8.0f;
        if (input.KeyDown(SDL_SCANCODE_LSHIFT)) { speed *= 0.1f; }
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
}
