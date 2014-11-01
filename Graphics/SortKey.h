#ifndef GRAPHICS_SORT_KEY_H
#define GRAPHICS_SORT_KEY_H

#include <cstdint>
#include "Material.h"

namespace Graphics
{
    class SortKey
    {
    public:
        uint64_t key;

        SortKey() : key(0) { }
        SortKey(BlendType blendType, float depth, uint32_t materialId, uint8_t pass);

        BlendType GetBlendType();
        void UpdateDepth(float depth);

        inline bool operator<(const SortKey& other) const
        {
            return key < other.key;
        }
    };
}

#endif
