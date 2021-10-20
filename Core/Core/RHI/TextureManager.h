#pragma once

namespace RHI
{
	class GraphicCore;

	using TEXTURE_HANDLE = size_t;
	inline constexpr TEXTURE_HANDLE INVALID_TEXTURE_HANDLE = TEXTURE_HANDLE(-1);

	class TextureManager
	{
	public:
		TextureManager(GraphicCore* gfx);

		void LoadTexture(const std::wstring& name);
		void LoadCubemap(const std::wstring& name);

		ID3D12Resource* GetResource(TEXTURE_HANDLE handle)
		{
			if (handle == INVALID_TEXTURE_HANDLE)
				return nullptr;
			else if (auto it = m_textureMap.find(handle); it != m_textureMap.end())
				return it->second.Get();
			else
				return nullptr;
		}

	private:
		GraphicCore* m_gfx;

		std::unordered_map<TEXTURE_HANDLE, Microsoft::WRL::ComPtr<ID3D12Resource>> m_textureMap;
	};
}