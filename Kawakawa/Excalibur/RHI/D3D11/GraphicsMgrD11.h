#pragma once
#include "../GraphicsMgr.h"
#include "../../Core/Math/KawaiiMath.h"
#include "../../Utils/Singleton.h"
#include <d3d11.h>
#include <Windows.h>

#define SAVE_RELEASE_DXOBJ(OBJ) {OBJ->Release();OBJ=0;}

namespace Excalibur {

	class GraphicsMgrD11 : public GraphicsManager {

	public:
		virtual		int		Initialize() noexcept;
		virtual		int		InitializeWithWindow(HWND handler) noexcept;
		virtual		void	Finalize() noexcept;
		virtual		void	Tick() noexcept;

		virtual		void	Present() noexcept;
		virtual		void	ClearRenderTarget(float r, float g, float b, float a) noexcept;

		virtual		std::shared_ptr<VertexBufferBase>	CreateVertexBuffer(void* data, int count, VertexFormat vf) noexcept;

		virtual		std::shared_ptr<IndexBufferBase>	CreateIndexBuffer(void* data, int count) noexcept;

		virtual		std::shared_ptr<MeshBase>		CreateRenderMesh(aiMesh* mesh, const aiScene* world) noexcept;
		virtual		std::shared_ptr<MeshBase>		CreateRenderMeshDebug(void* data, int count, VertexFormat vf) noexcept;

		virtual		std::shared_ptr<TextureBase>		CreateTexture2D(const std::string& path) noexcept;
		virtual		std::shared_ptr<SamplerState>	CreateSamplerState() noexcept;

		virtual		void	LoadShaders() noexcept;
		virtual		void	UseShader(std::shared_ptr<ShaderBase>) noexcept;
		virtual		std::shared_ptr<ShaderBase> GetShader(const std::string& shaderName) noexcept;

		virtual		void	Draw(unsigned int vcount, unsigned int start) noexcept;
		virtual		void	DrawIndexed(unsigned int icount, unsigned int start, int baseLoc) noexcept;

	public:
		HWND m_hwnd;

		char m_videoCardDescription[128];
		int m_videoCardMemory;

		IDXGISwapChain*				m_swapChain;
		ID3D11Device*				m_device;
		ID3D11DeviceContext*		m_deviceContext;
		ID3D11RenderTargetView*		m_renderTargetView;
		ID3D11Texture2D*			m_depthStencilBuffer;
		ID3D11DepthStencilState*	m_depthStencilState;
		ID3D11DepthStencilView*		m_depthStencilView;
		ID3D11RasterizerState*		m_rasterState;

	};

}