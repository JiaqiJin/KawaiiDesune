#include "pch.h"
#include "DX12Shader.h"

using namespace Microsoft::WRL;

namespace RHI
{
	// ---------------------------- TShaderDefines ---------------------------------------------
	// -------------------------------------------------------------------------------------
	void TShaderDefines::GetD3DShaderMacro(std::vector<D3D_SHADER_MACRO>& OutMacros) const
	{
		for (const auto& Pair : DefinesMap)
		{
			D3D_SHADER_MACRO Macro;
			Macro.Name = Pair.first.c_str();
			Macro.Definition = Pair.second.c_str();
			OutMacros.push_back(Macro);
		}

		D3D_SHADER_MACRO Macro;
		Macro.Name = NULL;
		Macro.Definition = NULL;
		OutMacros.push_back(Macro);
	}

	bool TShaderDefines::operator == (const TShaderDefines& Other) const
	{
		if (DefinesMap.size() != Other.DefinesMap.size())
		{
			return false;
		}

		for (const auto& Pair : DefinesMap)
		{
			const std::string Key = Pair.first;
			const std::string Value = Pair.second;

			auto Iter = Other.DefinesMap.find(Key);
			if (Iter == Other.DefinesMap.end() || Iter->second != Value)
			{
				return false;
			}
		}

		return true;
	}

	void TShaderDefines::SetDefine(const std::string& Name, const std::string& Definition)
	{
		DefinesMap.insert_or_assign(Name, Definition);
	}

	// ---------------------------- DX12Shader ---------------------------------------------
	// -------------------------------------------------------------------------------------
	DX12Shader::DX12Shader(const TShaderInfo& ShaderInfo, DX12GraphicRHI* GraphicRHI)
		: D3D12RHI(GraphicRHI)
	{

	}

	void DX12Shader::Initialize()
	{
		std::wstring ShaderDir = L"Resource/Shaders/";
		std::wstring FilePath = ShaderDir + L".hlsl";

		std::vector<D3D_SHADER_MACRO> ShaderMacros;
		m_ShaderInfo.ShaderDefines.GetD3DShaderMacro(ShaderMacros);

		if (m_ShaderInfo.bCreateVS)
		{
			auto VSBlob = CompileShader(FilePath, ShaderMacros.data(), m_ShaderInfo.VSEntryPoint, "vs_5_1");
			m_ShaderPass["VS"] = VSBlob;

			GetShaderParameters(VSBlob, EShaderType::VERTEX_SHADER);
		}

		if (m_ShaderInfo.bCreatePS)
		{
			auto PSBlob = CompileShader(FilePath, ShaderMacros.data(), m_ShaderInfo.PSEntryPoint, "ps_5_1");
			m_ShaderPass["PS"] = PSBlob;

			GetShaderParameters(PSBlob, EShaderType::PIXEL_SHADER);
		}

		if (m_ShaderInfo.bCreateCS)
		{
			auto CSBlob = CompileShader(FilePath, ShaderMacros.data(), m_ShaderInfo.CSEntryPoint, "cs_5_1");
			m_ShaderPass["CS"] = CSBlob;

			GetShaderParameters(CSBlob, EShaderType::COMPUTE_SHADER);
		}
	}

	Microsoft::WRL::ComPtr<ID3DBlob> DX12Shader::CompileShader(const std::wstring& Filename, const D3D_SHADER_MACRO* Defines,
		const std::string& Entrypoint, const std::string& Target)
	{
		UINT CompileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG) 
		// Enable better shader debugging with the graphics debugging tools.
		CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		HRESULT hr = S_OK;

		ComPtr<ID3DBlob> ByteCode = nullptr;
		ComPtr<ID3DBlob> Errors;
		hr = D3DCompileFromFile(Filename.c_str(), Defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			Entrypoint.c_str(), Target.c_str(), CompileFlags, 0, &ByteCode, &Errors);

		if (Errors != nullptr)
			OutputDebugStringA((char*)Errors->GetBufferPointer());

		ThrowIfFailed(hr);

		return ByteCode;
	}

	void DX12Shader::GetShaderParameters(ComPtr<ID3DBlob> PassBlob, EShaderType ShaderType)
	{
		ID3D12ShaderReflection* Reflection = NULL;
		D3DReflect(PassBlob->GetBufferPointer(), PassBlob->GetBufferSize(), IID_ID3D12ShaderReflection, (void**)&Reflection);

		D3D12_SHADER_DESC ShaderDesc;
		Reflection->GetDesc(&ShaderDesc);

		for (UINT i = 0; i < ShaderDesc.BoundResources; i++)
		{
			D3D12_SHADER_INPUT_BIND_DESC  ResourceDesc;
			Reflection->GetResourceBindingDesc(i, &ResourceDesc);

			auto ShaderVarName = ResourceDesc.Name;
			auto ResourceType = ResourceDesc.Type;
			auto RegisterSpace = ResourceDesc.Space;
			auto BindPoint = ResourceDesc.BindPoint;
			auto BindCount = ResourceDesc.BindCount;
			
			if (ResourceType == D3D_SHADER_INPUT_TYPE::D3D_SIT_CBUFFER)
			{
				
			}
			else if (ResourceType == D3D_SHADER_INPUT_TYPE::D3D_SIT_STRUCTURED
				|| ResourceType == D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE)
			{
			
			}
			else if (ResourceType == D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWSTRUCTURED
				|| ResourceType == D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWTYPED)
			{
				assert(ShaderType == EShaderType::COMPUTE_SHADER);

				
			}
			else if (ResourceType == D3D_SHADER_INPUT_TYPE::D3D_SIT_SAMPLER)
			{
				assert(ShaderType == EShaderType::PIXEL_SHADER);

			
			}
		}
	}
}