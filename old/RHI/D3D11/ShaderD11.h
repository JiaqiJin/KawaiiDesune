#pragma once

#include "../ShaderBase.h"
#include <d3d11.h>

namespace Excalibur 
{
	class ShaderD11 : public ShaderBase 
	{
	public:
		ShaderD11(const std::string & vsPath, const std::string & psPath);
		virtual ~ShaderD11();

		virtual	bool	InitializeFromFile(const std::string& vsPath, const std::string& psPath) noexcept;
		virtual	void	Use() noexcept;
		virtual void	Finialize() noexcept;
		virtual void	SetConstantBuffer(const ConstantBuffer& cbuffer) noexcept;

	public:
		ID3D11VertexShader*		m_vertexShader;
		ID3D11PixelShader*		m_pixelShader;
		ID3D11InputLayout*		m_layout;
		ID3D11Buffer*			m_matrixBuffer;

	};

}