#include "../Kawaiipch.h"
#include "Renderer.h"
#include "../RHI/GraphicsCore.h"

#include "pix3.h"

using namespace DirectX;
using namespace Microsoft::WRL;

Renderer::Renderer(RHI::GraphicCore* gfx, uint32_t width, uint32_t height)
	: gfx(gfx), width(width), height(height), m_backBufferCount(gfx->BackbufferCount())
{
	LoadShaders();
	CreatePipelineStateObjects();
	CreateDescriptorHeaps();
}

Renderer::~Renderer()
{
	gfx->WaitForGPU();
}

void Renderer::LoadShaders()
{
	{
		RHI::ShaderBlob vsBlob, psBlob;
		
	}
}

#pragma warning( push )
#pragma warning( disable : 6262)
void Renderer::CreatePipelineStateObjects()
{
	auto device = gfx->GetDevice();
	{
		ThrowIfFailed(device->CreateRootSignature)
	}
}

void Renderer::CreateDescriptorHeaps()
{

}