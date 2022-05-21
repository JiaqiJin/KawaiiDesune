#pragma once

#include "Math/Math.h"

struct KVertex
{
	KVertex() {}
    KVertex(const TVector3& p, const TVector3& n, const TVector3& t, const TVector2& uv) :
        Position(p),
        Normal(n),
        TangentU(t),
        TexC(uv) {}
    KVertex(float px, float py, float pz,
        float nx, float ny, float nz,
        float tx, float ty, float tz,
        float u, float v) :
        Position(px, py, pz),
        Normal(nx, ny, nz),
        TangentU(tx, ty, tz),
        TexC(u, v) {}

	TVector3 Position;
	TVector3 Normal;
	TVector3 TangentU;
	TVector2 TexC;
};