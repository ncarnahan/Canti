#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "DrawCall.h"
#include "SortKey.h"
#include <vector>
#include <unordered_map>

namespace Graphics
{
    class Renderer
    {
    private:
        struct DrawCallPair
        {
            SortKey key;
            DrawCall drawCall;
            inline bool operator<(const DrawCallPair& other) const { return key < other.key; }
            inline bool operator>(const DrawCallPair& other) const { return !(key < other.key); }
        };

        std::vector<DrawCallPair> _drawCalls;
        uint32_t _lastMaterialId;
        std::unordered_map<Material*, uint32_t> _materialMap;

        Matrix4x4 _viewMatrix;
        Matrix4x4 _projectionMatrix;
        Vector3 _eyePosition;

    public:
        bool sortEnabled;
        int ignoreCount;

        Renderer();
        
        SortKey CreateSortKey(float depth, Material* material, uint8_t pass);
        void Submit(SortKey key, DrawCall& drawCall);
        void Draw();

        void SetViewMatrix(Matrix4x4& matrix) { _viewMatrix = matrix; }
        void SetProjectionMatrix(Matrix4x4& matrix) { _projectionMatrix = matrix; }
        void SetEyePosition(Vector3 eyePosition) { _eyePosition = eyePosition; }
    };
}

#endif
