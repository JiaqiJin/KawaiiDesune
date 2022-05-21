#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Vertex.h"

class KMesh
{
public:
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;

	KMesh();
	KMesh(KMesh&&) = default;
	KMesh(const KMesh&) = delete;
	KMesh& operator=(const KMesh&) = delete;
	
private:


public:
	std::string Name;

	// Vertices
	std::vector<KVertex> m_Vertices;

	// Indices
	std::vector<uint32> m_Indices;
};