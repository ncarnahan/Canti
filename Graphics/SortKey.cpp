#include "SortKey.h"

namespace Graphics
{
    const uint64_t BLEND_BITS = 2;
    const uint64_t BLEND_MASK = (1 << BLEND_BITS) - 1;

    const uint64_t DEPTH_BITS = 24;
    const uint64_t DEPTH_MASK = (1 << DEPTH_BITS) - 1;

    const uint64_t MATERIAL_BITS = 24;
    const uint64_t MATERIAL_MASK = (1 << MATERIAL_BITS) - 1;

    const uint64_t PASS_BITS = 2;
    const uint64_t PASS_MASK = (1 << PASS_BITS) - 1;

    uint32_t FloatFlip(uint32_t value)
    {
        uint32_t mask = -int32_t(value >> 31) | 0x80000000;
        return value ^ mask;
    }

    uint64_t DepthConvert(float depth)
    {
        union { float f32; uint32_t u32; } value = { depth };
        value.u32 = FloatFlip(value.u32) >> (32 - DEPTH_BITS);
        return (uint64_t)value.u32;
    }



    SortKey::SortKey(BlendType blendType, float depth, uint32_t materialId, uint8_t pass)
    {
        uint64_t blendBits = ((uint64_t)blendType) & BLEND_MASK;
        blendBits <<= MATERIAL_BITS + DEPTH_BITS + PASS_BITS;

        uint64_t depthBits = DepthConvert(depth) << PASS_BITS;
        if (blendType == BlendType::Transparent)
        {
            //Flip 'em so that we sort back to front
            depthBits ^= DEPTH_MASK;
            depthBits <<= MATERIAL_BITS;
        }

        uint64_t materialBits = (materialId & MATERIAL_MASK) << PASS_BITS;
        if (blendType != BlendType::Transparent) { materialBits <<= DEPTH_BITS; }

        uint64_t passBits = pass & PASS_MASK;

        key = blendBits ^ depthBits ^ materialBits ^ passBits;
    }

    BlendType SortKey::GetBlendType()
    {
        return (BlendType)((key >> (MATERIAL_BITS + DEPTH_BITS)) & BLEND_MASK);
    }

    void SortKey::UpdateDepth(float depth)
    {
        int offset = 0;
        if (GetBlendType() == BlendType::Transparent) { offset = MATERIAL_BITS; }

        //Clear depth
        key &= (DEPTH_MASK << offset);

        //Write new depth
        uint64_t depthBits = DepthConvert(depth) << offset;
        key |= depthBits;
    }
}
