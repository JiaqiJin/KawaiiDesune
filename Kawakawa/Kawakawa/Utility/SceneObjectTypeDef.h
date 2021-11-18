#pragma once

#include <cstdint>
#include "../Math/KawaiiMath.h"
#include "Portable.h"

namespace Kawaii
{
    // Common Base Objt attributes types 
    enum struct SceneObjectType 
    {
        Mesh,
        Material,
        Texture,
        Light,
        Camera,
        Animator,
        Clip,
        Geometry,
        IndexArray,
        VertexArray,
    };

    enum struct SceneObjectCollisionType
    {
        None,
        Sphere,
        BOx,
        Cylinder,
        Capsule,
        Cone,
        MultiSphere,
        ConvexHull,
        ConvexMesh,
        BvhMesh,
        HeightField,
        P
    };

    enum struct VertexDataType 
    {
        Float1,
        Float2,
        Float3,
        Float4,
        Double1,
        Double2,
        Double3,
        Double4,
    };

    enum struct IndexDataType 
    {
        Int8,
        Int16,
        Int32,
        Int64,
    };

    enum struct PrimitiveType 
    {
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,
        LineListAdjacency,
        LineStripAdjacency,
        TriangleListAdjacency,
        TriangleStripAdjacency,
    };
}