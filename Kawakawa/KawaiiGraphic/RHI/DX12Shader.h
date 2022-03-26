#pragma once

#include "DX12Resource.h"
#include "DX12GraphicRHI.h"

using Microsoft::WRL::ComPtr;

namespace RHI
{
	enum class EShaderType
	{
		VERTEX_SHADER,
		PIXEL_SHADER,
		COMPUTE_SHADER,
	};

	struct TShaderDefines
	{
	public:
		void GetD3DShaderMacro(std::vector<D3D_SHADER_MACRO>& OutMacros) const;

		bool operator == (const TShaderDefines& Other) const;

		void SetDefine(const std::string& Name, const std::string& Definition);

	public:
		std::unordered_map<std::string, std::string> DefinesMap;
	};

	struct TShaderParameter
	{
		std::string Name;
		EShaderType ShaderType;
		UINT BindPoint;
		UINT RegisterSpace;
	};

	struct TShaderCBVParameter : TShaderParameter
	{
		
	};

	struct TShaderSRVParameter : TShaderParameter
	{
		UINT BindCount; //  Is the number of bind slots taken by the resource. 
	};

	struct TShaderUAVParameter : TShaderParameter
	{
		UINT BindCount; //  Is the number of bind slots taken by the resource. 
	};

	struct TShaderSamplerParameter : TShaderParameter
	{

	};

	struct TShaderInfo
	{
		std::string ShaderName;
		std::string FileName;
		TShaderDefines ShaderDefines;

		bool bCreateVS = false;
		std::string VSEntryPoint = "VS";

		bool bCreatePS = false;
		std::string PSEntryPoint = "PS";

		bool bCreateCS = false;
		std::string CSEntryPoint = "CS";
	};

	class DX12Shader
	{
	public:
		DX12Shader(const TShaderInfo& ShaderInfo, DX12GraphicRHI* GraphicRHI);

		void Initialize();
	private:
		static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const std::wstring& Filename, 
			const D3D_SHADER_MACRO* Defines, const std::string& Entrypoint, const std::string& Target);

		void GetShaderParameters(ComPtr<ID3DBlob> PassBlob, EShaderType ShaderType);

	public:
		TShaderInfo m_ShaderInfo;

		TShaderInfo m_ShaderInfo;

		std::vector<TShaderCBVParameter> CBVParams;
		std::vector<TShaderSRVParameter> SRVParams;
		std::vector<TShaderUAVParameter> UAVParams;
		std::vector<TShaderSamplerParameter> SamplerParams;

		UINT SRVCount = 0;
		UINT UAVCount = 0;

		int CBVSignatureBaseBindSlot = -1;
		int SRVSignatureBindSlot = -1;
		int UAVSignatureBindSlot = -1;
		int SamplerSignatureBindSlot = -1;

		std::unordered_map<std::string, ComPtr<ID3DBlob>> m_ShaderPass;
		ComPtr<ID3D12RootSignature> m_RootSignature;
	private:
		DX12GraphicRHI* D3D12RHI;
	};
}

namespace std
{
	template <>
	struct hash<RHI::TShaderDefines>
	{
		std::size_t operator()(const RHI::TShaderDefines& Defines) const
		{
			using std::size_t;
			using std::hash;
			using std::string;
			// Compute individual hash values for each string 
			// and combine them using XOR
			// and bit shifting:

			size_t HashValue = 0;
			for (const auto& Pair : Defines.DefinesMap)
			{
				HashValue ^= (hash<string>()(Pair.first) << 1);
				HashValue ^= (hash<string>()(Pair.second) << 1);
			}

			return HashValue;
		}
	};
}