#pragma once

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

	struct InputLayout
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> IE_Desc;
	};

	namespace ShaderUtil
	{
		void CompileShader();
	}
}