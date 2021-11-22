#include "RootSignature.h"

namespace Kawaii::Graphics::backend::DX12
{
	RootSignature::RootSignature(std::string_view name, uint32_t num_root_params, uint32_t num_static_samplers)
		: m_Name(name),
		m_RootSignature(nullptr),
		m_NumParameters(num_root_params),
		m_NumSamplers(num_static_samplers),
		m_NumInitializedStaticSamplers(num_static_samplers)
	{
		Reset(num_root_params, num_static_samplers);
	}

	void RootSignature::Reset(uint32_t num_root_params, uint32_t num_static_samplers)
	{

	}

	void RootSignature::Finalize(ID3D12Device* device, D3D12_ROOT_SIGNATURE_FLAGS flags, D3D_ROOT_SIGNATURE_VERSION version)
	{
		if (m_Finalized) return;

		for (size_t i = 0; i < m_NumParameters; i++)
		{
			switch (m_ParamArray[i].ParameterType)
			{
			case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
				m_NumDescriptorTables++;
				break;
			case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
				m_NumRootConstants++;
				break;
			default:
				m_NumRootDescriptors++;
				break;
			}
		}

		m_RootSignatureDesc.NumParameters = m_NumParameters;
		m_RootSignatureDesc.pParameters = m_ParamArray.data();
		m_RootSignatureDesc.NumStaticSamplers = m_NumSamplers;
		m_RootSignatureDesc.pStaticSamplers = m_SamplerArray.data();
		m_RootSignatureDesc.Flags = flags;

		for (uint32_t root_index = 0; root_index < m_NumParameters; ++root_index)
		{
			const auto& RootParam = m_RootSignatureDesc.pParameters[root_index];
			m_NumDescriptorsPerTable[root_index] = 0;

			// Each descriptor table stores descriptors of one or more types - SRVs, UAVs, CBVs, and Samplers
			if (RootParam.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
			{
				assert(RootParam.DescriptorTable.pDescriptorRanges != nullptr);
				// sampler descriptor and cbv_srv_uav descriptor can not be in the same descriptor table.
				if (RootParam.DescriptorTable.pDescriptorRanges->RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
					m_SamplerTableBitMask |= (1 << root_index);
				else
					m_DescriptorTableBitMask |= (1 << root_index);

				for (uint32_t range_index = 0; range_index < RootParam.DescriptorTable.NumDescriptorRanges; range_index++)
					m_NumDescriptorsPerTable[root_index] += RootParam.DescriptorTable.pDescriptorRanges[range_index].NumDescriptors;
			}
		}

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC version_root_signature_desc(m_RootSignatureDesc);

		// Serialize the root signature.
		Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

		if (FAILED(D3DX12SerializeVersionedRootSignature(&version_root_signature_desc, version, &serializedRootSig, &errorBlob)))
		{
			auto buffer = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());
			auto size = errorBlob->GetBufferSize();
			std::string info(buffer, size);
			std::cout << info << std::endl;
			throw std::runtime_error("Serialize Root RootSignature failed.");
		}

		ThrowIfFailed(device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));

		m_RootSignature->SetName(std::wstring(m_Name.begin(), m_Name.end()).data());

		m_Finalized = true;
	}
}