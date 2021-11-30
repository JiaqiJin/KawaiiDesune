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


    std::shared_ptr<LinearAllocationPage> PageManager::RequesetPage(ID3D12Device* device)
    {
        std::lock_guard lock(m_Mutex);
        {
            if (!m_AvailablePages.empty())
            {
                auto page = std::move(m_AvailablePages.front());
                m_AvailablePages.pop();
                return page;
            }
        }// unlock mutex

        return CreateNewPage(device, m_PageType, m_DefaultSize);
    }

    std::shared_ptr<LinearAllocationPage> PageManager::RequesetLargePage(ID3D12Device* device, size_t size)
    {
        assert(size > m_DefaultSize);
        return CreateNewPage(device, m_PageType, size);
    }

    void PageManager::DiscardPage(std::shared_ptr<LinearAllocationPage> page, uint64_t FenceID)
    {
        std::lock_guard lock(m_Mutex);
        m_RetiredPages.emplace_back(std::move(page), FenceID);
    }

    void PageManager::DiscardLargePage(std::shared_ptr<LinearAllocationPage> page, uint64_t FenceID)
    {
        std::lock_guard lock(m_Mutex);
        m_LargePages.emplace_back(std::move(page), FenceID);
    }

    void PageManager::Reset()
    {
        std::lock_guard lock(m_Mutex);
        m_RetiredPages.clear();
        m_AvailablePages = {};  // use a empty queue to clear
        m_LargePages.clear();
    }

    void PageManager::UpdateAvailablePages(std::function<bool(uint64_t)>&& fence_checker)
    {
        std::lock_guard lock(m_Mutex);
    
        for (auto iter = m_RetiredPages.begin(); iter != m_RetiredPages.end();)
        {
            auto&& [page, fence] = *iter;
            if (fence_checker(fence) && page->m_allocation_count == 0)
            {
                page->m_offset = 0;
                m_AvailablePages.emplace(std::move(page));
                iter = m_RetiredPages.erase(iter);
            }
            else
                iter++;
        }

        for (auto iter = m_LargePages.begin(); iter != m_LargePages.end();) {
            auto&& [page, fence] = *iter;
            if (fence_checker(fence)) {
                iter = m_LargePages.erase(iter);
            }
            else {
                iter++;
            }
        }
    }

    // --------------------------- Lineal Allocator-------------------------------
    Allocation LinearAllocator::Allocate(size_t size, size_t alignment)
    {
        size_t aligned_size = align(size, alignment);
        auto& mgr = sm_PageManager[static_cast<size_t>(m_Type)];

        // need large page
        if (aligned_size > GetDefaultSize(m_Type))
        {
            auto& page = m_LargePages.emplace_back(mgr.RequesetLargePage(m_Device, aligned_size));
            return { page, 0, aligned_size, page->m_CpuPtr, (*page)->GetGPUVirtualAddress() };
        }

        // use default size page
        // requeset new page
        if (m_CurrPage == nullptr || aligned_size > m_CurrPage->GetFreeSize())
            m_CurrPage = m_Pages.emplace_back(mgr.RequesetPage(m_Device));

        size_t offset = m_CurrPage->m_offset;
        uint8_t* cpu_ptr = m_CurrPage->m_CpuPtr + m_CurrPage->m_offset;
        D3D12_GPU_VIRTUAL_ADDRESS gpu_ptr = m_CurrPage->m_Resource->GetGPUVirtualAddress() + m_CurrPage->m_offset;

        m_CurrPage->m_offset += size;
        m_CurrPage->m_allocation_count++;

        return { m_CurrPage, offset, size, cpu_ptr, gpu_ptr };
    }

    // Discard all page already in effect with fence value
    void LinearAllocator::SetFence(uint64_t fence)
    {
        // no allocation occur, just return.
        auto& mgr = sm_PageManager[static_cast<size_t>(m_Type)];
        for (auto&& page : m_Pages)
            mgr.DiscardPage(std::move(page), fence);
        m_Pages.clear();

        for (auto&& page : m_LargePages)
            mgr.DiscardLargePage(std::move(page), fence);
        m_LargePages.clear();
    }
}