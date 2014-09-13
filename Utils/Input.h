#ifndef UTILS_INPUT_H
#define UTILS_INPUT_H

#include <SDL.h>

namespace Utils
{
    class Input
    {
    private:
        static const size_t KEY_COUNT = SDL_NUM_SCANCODES;
        bool currentKeys[KEY_COUNT];
        bool previousKeys[KEY_COUNT];

        static const size_t BUTTON_COUNT = 5;
        bool currentButtons[BUTTON_COUNT];
        bool previousButtons[BUTTON_COUNT];

        int relativeX, relativeY;

    public:
        void Update();

        bool KeyDown(int key);
        bool KeyPressed(int key);
        bool KeyReleased(int key);

        bool ButtonDown(int button);
        bool ButtonPressed(int button);
        bool ButtonReleased(int button);

        inline int GetRelativeMouseX() { return relativeX; }
        inline int GetRelativeMouseY() { return relativeY; }
    };
}

#endif
