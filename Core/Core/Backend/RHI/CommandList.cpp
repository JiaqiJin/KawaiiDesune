#include "../../Kawaiipch.h"

#include "CommandList.h"
#include "Device.h"

namespace Kawaii
{
    CommandList::CommandList()
    {
    }

    CommandList::~CommandList()
    {

    }

    bool CommandList::Initialize(ID3D12GraphicsCommandList6* commandList)
    {
        m_CommandList = commandList;
        return true;
    }

    // -------------------- Graphic CommandList --------------------
    bool GraphicCommandList::Initialize(Device& device, ID3D12CommandAllocator* allocator, ID3D12PipelineState* PSO)
    {
        ID3D12GraphicsCommandList6* commandList = nullptr;
        HRESULT hr = device.GetD3D12Device()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, PSO,
            IID_PPV_ARGS(&commandList));

        if (FAILED(hr))
            return false;

        return CommandList::Initialize(commandList);
    }

    bool GraphicCommandList::Reset(ID3D12CommandAllocator* allocator, ID3D12PipelineState* PSO)
    {
        HRESULT hr = m_CommandList->Reset(allocator, PSO);
        return SUCCEEDED(hr);
    }

    // -------------------- Copy CommandList --------------------
    bool CopyCommandList::Initialize(Device& device, ID3D12CommandAllocator* allocator, ID3D12PipelineState* PSO)
    {
        ID3D12GraphicsCommandList6* commandList = nullptr;
        HRESULT hr = device.GetD3D12Device()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COPY, allocator, PSO,
            IID_PPV_ARGS(&commandList));

        if (FAILED(hr))
            return false;

        return CommandList::Initialize(commandList);
    }

    bool CopyCommandList::Reset(ID3D12CommandAllocator* allocator, ID3D12PipelineState* PSO)
    {
        HRESULT hr = m_CommandList->Reset(allocator, PSO);
        return SUCCEEDED(hr);
    }

    // -------------------- Compute CommandList --------------------
    bool ComputeCommandList::Initialize(Device& device, ID3D12CommandAllocator* allocator, ID3D12PipelineState* PSO)
    {
        ID3D12GraphicsCommandList6* commandList = nullptr;
        HRESULT hr = device.GetD3D12Device()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, allocator, PSO,
            IID_PPV_ARGS(&commandList));

        if (FAILED(hr))
            return false;

        return CommandList::Initialize(commandList);
    }

    bool ComputeCommandList::Reset(ID3D12CommandAllocator* allocator, ID3D12PipelineState* PSO)
    {
        HRESULT hr = m_CommandList->Reset(allocator, PSO);
        return SUCCEEDED(hr);
    }
}