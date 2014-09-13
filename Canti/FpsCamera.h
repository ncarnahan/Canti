#ifndef CANTI_FPS_CAMERA_H
#define CANTI_FPS_CAMERA_H

#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Quaternion.h>
#include <Math/Matrix4x4.h>
using namespace Math;
#include <Utils/Input.h>
using namespace Utils;

class FpsCamera
{
private:
    Vector3 _position;
    Vector2 _rotation;

    SDL_Window* _window;
    int _mouseDownX;
    int _mouseDownY;

public:
    inline FpsCamera()
    {
    }

    void Update(float dt, Input& input);


    inline void SetWindow(SDL_Window* window)
    {
        _window = window;
    }

    inline void SetPosition(Vector3 position)
    {
        _position = position;
    }

    inline Vector3 GetPosition() { return _position; }
    inline Quaternion GetRotation()
    {
        return Quaternion::AngleAxis(_rotation.x, Vector3::up) *
            Quaternion::AngleAxis(_rotation.y, Vector3::right);
    }

    inline Matrix4x4 GetViewMatrix()
    {
        return Matrix4x4::FromTransform(_position, GetRotation(), Vector3::one).GetInverse();
    }
};

#endif
