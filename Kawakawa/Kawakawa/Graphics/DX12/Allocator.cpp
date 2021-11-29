#include "D3Dpch.h"
#include "Allocator.h"
#include "../../Math/KawaiiMath.h"

namespace Kawaii::Graphics::backend::DX12
{
    Allocation::~Allocation()
    {
        if (auto page = page_from.lock())   
            page->DiscardAllocation(*this);
    }

    AllocationPage::AllocationPage(AllocationPage&& rhs)
        : GpuResource(std::move(rhs)),
        m_Size(rhs.m_Size),
        m_CpuPtr(rhs.m_CpuPtr) 
    {
        rhs.m_CpuPtr = nullptr;
    }

    AllocationPage& AllocationPage::operator=(AllocationPage&& rhs)
    {
        if (&rhs != this) {
            GpuResource::operator=(std::move(rhs));
            m_Size = rhs.m_Size;
            m_CpuPtr = rhs.m_CpuPtr;
            rhs.m_Size = 0;
            rhs.m_CpuPtr = nullptr;
        }
        return *this;
    };

	// -------------------------------------- Page Manager -----------------------------------------
	std::shared_ptr<LinearAllocationPage> PageManager::CreateNewPage(ID3D12Device* device, AllocationPageType type, size_t size)
	{
        // Heap prop
        D3D12_HEAP_PROPERTIES HeapProps;
        HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        HeapProps.CreationNodeMask = 1;
        HeapProps.VisibleNodeMask = 1;

        // Resouce Desc
        D3D12_RESOURCE_DESC ResourceDesc;
        ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        ResourceDesc.Alignment = 0;
        ResourceDesc.Height = 1;
        ResourceDesc.DepthOrArraySize = 1;
        ResourceDesc.MipLevels = 1;
        ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        ResourceDesc.SampleDesc.Count = 1;
        ResourceDesc.SampleDesc.Quality = 0;
        ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        // Resouce State
        D3D12_RESOURCE_STATES DefaultUsage;

        switch (type)
        {
        case AllocationPageType::GPU_EXCLUSIVE:
        {
            HeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
            ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
            DefaultUsage = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
        } break;
        case AllocationPageType::CPU_WRITABLE:
        {
            HeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
            ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size, D3D12_RESOURCE_FLAG_NONE);
            DefaultUsage = D3D12_RESOURCE_STATE_GENERIC_READ;
        } break;
        default:
            throw std::bad_alloc();
            break;
        }

        ID3D12Resource* resource = nullptr;
        ThrowIfFailed(device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, 
            &ResourceDesc, DefaultUsage, nullptr, IID_PPV_ARGS(&resource)));
        resource->SetName(L"Linear Allocation Page");
        // resource ownership transfer to GpuResource
        auto ret = std::make_shared<LinearAllocationPage>(GpuResource(resource), size);
        return ret;
	}
}