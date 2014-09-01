#include "Input.h"

namespace Utils
{
    void Input::Update()
    {
        auto state = SDL_GetKeyboardState(NULL);
        for (size_t i = 0; i < KEY_COUNT; i++)
        {
            previousKeys[i] = currentKeys[i];
            currentKeys[i] = (state[i] != 0);
        }

        int buttons = SDL_GetRelativeMouseState(&relativeX, &relativeY);
        for (size_t i = 0; i < BUTTON_COUNT; i++)
        {
            previousButtons[i] = currentButtons[i];
            currentButtons[i] = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT + i)) != 0;
        }
    }


    bool Input::KeyDown(int key)
    {
        return currentKeys[key];
    }

    bool Input::KeyPressed(int key)
    {
        return currentKeys[key] && !previousKeys[key];
    }

    bool Input::KeyReleased(int key)
    {
        return !currentKeys[key] && previousKeys[key];
    }


    bool Input::ButtonDown(int button)
    {
        return currentButtons[button];
    }

    bool Input::ButtonPressed(int button)
    {
        return currentButtons[button] && !previousButtons[button];
    }

    bool Input::ButtonReleased(int button)
    {
        return !currentButtons[button] && previousButtons[button];
    }
}