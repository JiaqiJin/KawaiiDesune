#pragma comment(lib, "dxcompiler.lib")
#include "../Kawaiipch.h"

#include "ShaderUtil.h"
#include "../Utils/StringUtil.h"

#include <wrl.h>
#include <d3dcompiler.h> 
#include <dxc/dxcapi.h>

using namespace Microsoft::WRL;

namespace RHI
{
	// Shader Reflection
	class ReflectionBlob : public IDxcBlob
	{
	public:

	private:

	};

	namespace 
	{
		Microsoft::WRL::ComPtr<IDxcLibrary> library = nullptr;
		Microsoft::WRL::ComPtr<IDxcCompiler> compiler = nullptr;
		Microsoft::WRL::ComPtr<IDxcIncludeHandler> include_handler = nullptr;
	}

	namespace ShaderUtil
	{
		void Init()
		{
			ThrowIfFailed(DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library)));

			ThrowIfFailed(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler)));

			ThrowIfFailed(library->CreateIncludeHandler(&include_handler));
		}

		void Destroy()
		{
			include_handler.Reset();
			compiler.Reset();
			library.Reset();
		}

		// https://asawicki.info/news_1719_two_shader_compilers_of_direct3d_12
		void CompileShader(ShaderInfo const& input, ShaderBlob& blob)
		{
			// Params
			Microsoft::WRL::ComPtr<IDxcBlob> _blob;
			uint32_t codePage = CP_UTF8;
			Microsoft::WRL::ComPtr<IDxcBlobEncoding> sourceBlob;

			// Result
			HRESULT hr;

			// Shader source file string
			std::wstring shader_source = Util::ConvertToWide(input.shadersource);
		
			ThrowIfFailed(library->CreateBlobFromFile(shader_source.data(), &codePage, &sourceBlob));

			// Flags
			std::vector<wchar_t const*> flags{};
			if (input.flags & ShaderInfo::FLAG_DEBUG)
			{
				flags.push_back(L"-Zi");			//Debug info
				flags.push_back(L"-Qembed_debug");	//Embed debug info into the shader
			}
			if (input.flags & ShaderInfo::FLAG_DISABLE_OPTIMIZATION)
				flags.push_back(L"-Od");
			else flags.push_back(L"-O3");

			// Entry Poitn and Target
			std::wstring p_target = L"";
			std::wstring entry_point = L"";

			// Check input stage(VS, PS etc)
			switch (input.stage)
			{
			case ShaderStage::VS:
				p_target = L"vs_6_0";
				entry_point = L"vs_main";
				break;
			case ShaderStage::PS:
				p_target = L"ps_6_0";
				entry_point = L"ps_main";
				break;
			case ShaderStage::CS:
				p_target = L"cs_6_0";
				entry_point = L"cs_main";
				break;
			case ShaderStage::GS:
				p_target = L"gs_6_0";
				entry_point = L"gs_main";
				break;
			case ShaderStage::HS:
				p_target = L"hs_6_0";
				entry_point = L"hs_main";
				break;
			case ShaderStage::DS:
				p_target = L"ds_6_0";
				entry_point = L"ds_main";
				break;
			case ShaderStage::LIB:
				p_target = L"lib_6_3";
				break;
			default:
				assert(false && "Invalid Shader Stage");
			}

			if (!input.entrypoint.empty()) entry_point = Util::ConvertToWide(input.entrypoint);

			std::vector<DxcDefine> sm6_defines{};
			for (auto const& define : input.defines)
			{
				DxcDefine sm6_define{};
				sm6_define.Name = define.name.c_str();
				sm6_define.Value = define.value.c_str();
				sm6_defines.push_back(sm6_define);
			}

			// Compile shader 
			Microsoft::WRL::ComPtr<IDxcOperationResult> result;
			hr = compiler->Compile(
				sourceBlob.Get(),									// pSource
				shader_source.data(),								// pSourceName
				entry_point.c_str(),								// pEntryPoint
				p_target.c_str(),									// pTargetProfile
				flags.data(), (UINT32)flags.size(),					// pArguments, argCount
				sm6_defines.data(), (UINT32)sm6_defines.size(),	// pDefines, defineCount
				include_handler.Get(),					// pIncludeHandler
				&result);								// ppResult

			if (SUCCEEDED(hr))
				result->GetStatus(&hr);
			// Error reported
			if (FAILED(hr) && result)
			{
				Microsoft::WRL::ComPtr<IDxcBlobEncoding> errorsBlob;
				hr = result->GetErrorBuffer(&errorsBlob);
				if (SUCCEEDED(hr) && errorsBlob)
					wprintf(L"Compilation failed with errors:\n%hs\n", (const char*)errorsBlob->GetBufferPointer());
			}

			result->GetResult(_blob.GetAddressOf());
			blob.bytecode.resize(_blob->GetBufferSize());
			memcpy(blob.GetPointer(), _blob->GetBufferPointer(), blob.GetLength());
		}
	} // End namespace Shader Utility
}