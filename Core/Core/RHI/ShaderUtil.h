#pragma once

#include <string>
#include <vector>
#include <d3d12.h>

namespace RHI
{
	enum class ShaderStage
	{
		VS,
		PS,
		HS,
		DS,
		GS,
		CS,
		LIB,
		COUNT
	};

	struct ShaderBlob
	{
		std::vector<uint8_t> bytecode;

		void* GetPointer() const
		{
			return (void*)bytecode.data();
		}

		size_t GetLength() const
		{
			return bytecode.size();
		}

		operator D3D12_SHADER_BYTECODE() const
		{
			D3D12_SHADER_BYTECODE Bytecode{};
			Bytecode.pShaderBytecode = GetPointer();
			Bytecode.BytecodeLength = GetLength();
			return Bytecode;
		}
	};

	struct ShaderDefine
	{
		std::wstring name;
		std::wstring value;
	};

	struct ShaderInfo
	{
		enum FLAGS
		{
			FLAG_NONE = 0,
			FLAG_DEBUG = 1 << 0,
			FLAG_DISABLE_OPTIMIZATION = 1 << 1,
		};

		ShaderStage stage = ShaderStage::COUNT;
		std::string shadersource = "";
		std::string entrypoint = "";
		std::vector<ShaderDefine> defines;
		UINT64 flags = FLAG_NONE;
	};

	struct InputLayout
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> IE_Desc;
	};

	namespace ShaderUtil
	{
		void Init();

		void Destroy();

		void CompileShader(ShaderInfo const& input, ShaderBlob& blob);
	}
}