#pragma once
#include <memory>
#include <DirectXCollision.h>

#include "../RHI/Texture2D.h"

namespace RHI
{
	class GraphicCore;
	class IndexBuffer;
	class VertexBuffer;
	class Texture2D;
	class DescriptorHeap;
}

class Renderer
{
public:

private:
	RHI::GraphicCore* gfx;

	// Textures
	RHI::Texture2D m_texture;

	// Descriptor Heap
	std::unique_ptr<RHI::DescriptorHeap> m_rtvHeap;
	std::unique_ptr<RHI::DescriptorHeap> m_srvHeap;
	std::unique_ptr<RHI::DescriptorHeap> m_dsvHeap;
	std::unique_ptr<RHI::DescriptorHeap> m_uavHeap;

	uint32_t m_srvHeapIndex = 0;
	uint32_t m_uavheapIndex = 0;
	uint32_t m_rtvheapIndex = 0;
	uint32_t m_dsvheapIndex = 0;
};