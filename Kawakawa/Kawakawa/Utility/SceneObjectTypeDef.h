#pragma once

#include <cstdint>
#include "../Math/KawaiiMath.h"
#include "Portable.h"

namespace Kawaii
{
    enum struct SceneObjectType : int32_t 
    {
        MESH = "MESH"_i32,
        MATERIAL = "MATL"_i32,
        TEXTURE = "TXTU"_i32,
        LIGHT = "LGHT"_i32,
        CAMERA = "CAMR"_i32,
        ANIMATOR = "ANIM"_i32,
        CLIP = "CLIP"_i32,
        GEOMETRY = "GEOM"_i32,
        INDEX_ARRAY = "VARR"_i32,
        VERETEX_ARRAY = "VARR"_i32,
    };

}