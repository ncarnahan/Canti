#ifndef CANTI_ENTITY_H
#define CANTI_ENTITY_H

#include <Graphics/Material.h>
#include <Graphics/StaticMesh.h>
#include <Graphics/SortKey.h>
using namespace Graphics;
#include <Math/Vector3.h>
#include <Math/Quaternion.h>
using namespace Math;

struct Entity
{
    StaticMesh* mesh;
    Material* material;
    Material* deferredMaterial;
    SortKey sortKey;

    Vector3 position;
    Quaternion rotation;
    float scale;

    Entity();
};

#endif
