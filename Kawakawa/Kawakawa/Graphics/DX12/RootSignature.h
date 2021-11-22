#pragma once

#include "D3Dpch.h"
#include "../Resource.h"

namespace Kawaii::Graphics::backend::DX12
{
	struct RootParameter : public CD3DX12_ROOT_PARAMETER1
	{
        using CD3DX12_ROOT_PARAMETER1::CD3DX12_ROOT_PARAMETER1;

		~RootParameter() 
		{ 
			if (ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
                delete[] DescriptorTable.pDescriptorRanges;
		}

        /*void InitAsConstants(UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL, UINT Space = 0)
        {
            ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
            ShaderVisibility = Visibility;
            Constants.Num32BitValues = NumDwords;
            Constants.ShaderRegister = Register;
            Constants.RegisterSpace = Space;
        }

        void InitAsConstantBuffer(UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL, UINT Space = 0)
        {
            ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            ShaderVisibility = Visibility;
            Descriptor.ShaderRegister = Register;
            Descriptor.RegisterSpace = Space;
        }

        void InitAsBufferSRV(UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL, UINT Space = 0)
        {
            ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
            ShaderVisibility = Visibility;
            Descriptor.ShaderRegister = Register;
            Descriptor.RegisterSpace = Space;
        }

        void InitAsBufferUAV(UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL, UINT Space = 0)
        {
            ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
            ShaderVisibility = Visibility;
            Descriptor.ShaderRegister = Register;
            Descriptor.RegisterSpace = Space;
        }*/

        void InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT shader_register, UINT count, UINT space = 0,
            D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
        {
            InitAsDescriptorTable(1, visibility);
            SetTableRange(0, type, shader_register, count, space);
        }

        void InitAsDescriptorTable(UINT range_count, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
        {
            auto* range = new D3D12_DESCRIPTOR_RANGE1[range_count];
            CD3DX12_ROOT_PARAMETER1::InitAsDescriptorTable(range_count, range, visibility);
        }

        void SetTableRange(UINT range_index, D3D12_DESCRIPTOR_RANGE_TYPE type, UINT shader_register, UINT count, UINT space = 0,
            D3D12_DESCRIPTOR_RANGE_FLAGS flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE)
        {
            assert(range_index < DescriptorTable.NumDescriptorRanges);

            D3D12_DESCRIPTOR_RANGE1& range =
                *const_cast<D3D12_DESCRIPTOR_RANGE1*>(DescriptorTable.pDescriptorRanges + range_index);
            range = CD3DX12_DESCRIPTOR_RANGE1(type, count, shader_register, space, flags);
        }
	};

    class RootSignature : public backend::Resource
    {
    public:
        RootSignature(std::string_view name, uint32_t num_root_params = 0, uint32_t num_static_samplers = 0);
        ~RootSignature() override = default;
        RootSignature(const RootSignature&) = delete;
        RootSignature& operator=(const RootSignature&) = delete;
        RootSignature(RootSignature&&) = default;
        RootSignature& operator=(RootSignature&&) = default;

        void Reset(uint32_t num_root_params, uint32_t num_static_samplers);

        // Getters
        ID3D12RootSignature* GetRootSignature() const { return m_RootSignature.Get(); }
        const D3D12_ROOT_SIGNATURE_DESC1& GetRootSignatureDesc() const { return m_RootSignatureDesc; }
        void Finalize(
            ID3D12Device* device,
            D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE,
            D3D_ROOT_SIGNATURE_VERSION version = D3D_ROOT_SIGNATURE_VERSION_1_1);

    private:
        std::string m_Name;
        bool m_Finalized = false;

        D3D12_ROOT_SIGNATURE_DESC1 m_RootSignatureDesc = {};
        ComPtr<ID3D12RootSignature> m_RootSignature;

        UINT m_NumParameters;
        UINT m_NumSamplers;
        UINT m_NumInitializedStaticSamplers;

        std::vector<RootParameter> m_ParamArray;
        std::vector<D3D12_STATIC_SAMPLER_DESC> m_SamplerArray;

        // One bit is set for root parameters that are non-sampler descriptor tables
        uint32_t m_DescriptorTableBitMask = 0;
        // One bit is set for root parameters that are sampler descriptor tables
        uint32_t m_SamplerTableBitMask = 0;
        // Non-sampler descriptor tables need to know their descriptor count
        std::array<uint32_t, 32> m_NumDescriptorsPerTable{};

        UINT m_NumDescriptorTables = 0; // Tex size
        UINT m_NumRootConstants = 0; // Constant size
        UINT m_NumRootDescriptors = 0;
    };
}