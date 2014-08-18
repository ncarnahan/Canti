#include "Vector3.h"

namespace Math
{
    const Vector3 Vector3::zero = Vector3(0, 0, 0);
    const Vector3 Vector3::right = Vector3(1, 0, 0);
    const Vector3 Vector3::left = Vector3(-1, 0, 0);
    const Vector3 Vector3::up = Vector3(0, 1, 0);
    const Vector3 Vector3::down = Vector3(0, -1, 0);
    const Vector3 Vector3::forward = Vector3(0, 0, 1);
    const Vector3 Vector3::back = Vector3(0, 0, -1);
}