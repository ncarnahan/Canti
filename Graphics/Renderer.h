#include "DrawCall.h"
#include <vector>

namespace Graphics
{
    class Renderer
    {
    private:
        std::vector<DrawCall> _drawCalls;
        Matrix4x4 _viewMatrix;
        Matrix4x4 _projectionMatrix;
        Vector3 _eyePosition;

    public:
        Renderer();

        void Submit(DrawCall& drawCall);
        void Draw();

        void SetViewMatrix(Matrix4x4& matrix) { _viewMatrix = matrix; }
        void SetProjectionMatrix(Matrix4x4& matrix) { _projectionMatrix = matrix; }
        void SetEyePosition(Vector3 eyePosition) { _eyePosition = eyePosition; }
    };
}
