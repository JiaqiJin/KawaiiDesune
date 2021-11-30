#pragma once
#include "D3Dpch.h"
#include "CommandListManager.h"
#include "GpuResource.h"

namespace Kawaii::Graphics::backend::DX12
{
	class AllocationPage;

	struct Allocation
	{
		Allocation(std::weak_ptr<AllocationPage> page_from, size_t page_offset, size_t size, uint8_t* cpu_ptr, D3D12_GPU_VIRTUAL_ADDRESS gpu_addr)
			:page_from(page_from)
		{}
		Allocation(const Allocation&) = delete;
		Allocation& operator=(const Allocation&) = delete;

		~Allocation();

		std::weak_ptr<AllocationPage> page_from;
		size_t                        page_offset;
		size_t                        size;
		uint8_t*					  cpu_ptr;
		D3D12_GPU_VIRTUAL_ADDRESS     gpu_addr;
	};

	class AllocationPage : public GpuResource
	{
	public:
		AllocationPage(const AllocationPage&) = delete;
		AllocationPage& operator=(const AllocationPage&) = delete;
		AllocationPage(AllocationPage&&);
		AllocationPage& operator=(AllocationPage&&);

		~AllocationPage() override
		{
			if (m_CpuPtr) 
				m_Resource->Unmap(0, nullptr);
		}

		// when a allocation from this page is destructing, that will call this function.
		virtual void DiscardAllocation(Allocation& allocation) = 0;

		// check is all the allocation from this page is destructed.
		virtual bool IsPageFree() const = 0;

	protected:
		AllocationPage(GpuResource&& resource, size_t size) : GpuResource(std::move(resource)), m_Size(size)
		{
			m_Resource->Map(0, nullptr, reinterpret_cast<void**>(&m_CpuPtr));
		}

		size_t m_Size;
		uint8_t* m_CpuPtr = nullptr;
	};

	enum class AllocationPageType
	{
		GPU_EXCLUSIVE = 0,  // Default
		CPU_WRITABLE = 1,  // cpu writable
		NUM_TYPES
	};

	// Lineal Allocation Page
	class LinearAllocationPage : public AllocationPage
	{
		friend class LinearAllocator;
		friend class PageManager;
	public:
		LinearAllocationPage(GpuResource&& resource, size_t size) : AllocationPage(std::move(resource), size) {}
		LinearAllocationPage(LinearAllocationPage&&) = default;
		LinearAllocationPage& operator=(LinearAllocationPage&&) = default;

		// when a allocation from this page is destructing, that will call this function.
		void DiscardAllocation(Allocation& allocation) final { m_allocation_count--; }

		size_t GetFreeSize() const noexcept { return m_Size - m_offset; }
		// check is all the allocation from this page is destructed.
		bool IsPageFree() const { return m_allocation_count == 0; }

	private:
		size_t m_offset = 0;
		size_t m_allocation_count = 0;
	};


	// Lineal Allocation Page Manager
	class PageManager
	{
	public:
		PageManager(AllocationPageType type, size_t default_page_size)
			: m_PageType(type), m_DefaultSize(default_page_size) {}

		static std::shared_ptr<LinearAllocationPage> CreateNewPage(ID3D12Device* device, AllocationPageType type, size_t size);

		void UpdateAvailablePages(std::function<bool(uint64_t)>&& fence_checker);

		std::shared_ptr<LinearAllocationPage> RequesetPage(ID3D12Device* device);
		std::shared_ptr<LinearAllocationPage> RequesetLargePage(ID3D12Device* device, size_t size);
		
		// get page ownership from caller
		void DiscardPage(std::shared_ptr<LinearAllocationPage> page, uint64_t FenceID);
		void DiscardLargePage(std::shared_ptr<LinearAllocationPage> page, uint64_t FenceID);
		void Reset();
	private:
		std::mutex m_Mutex;
		AllocationPageType m_PageType;
		size_t m_DefaultSize;

		std::list<std::pair<std::shared_ptr<LinearAllocationPage>, uint64_t>> m_RetiredPages;
		std::queue<std::shared_ptr<LinearAllocationPage>> m_AvailablePages;

		// When fence is complete, the large page will be delete
		std::list<std::pair<std::shared_ptr<LinearAllocationPage>, uint64_t>> m_LargePages;
	};


	// Lineal Allocator
	class LinearAllocator
	{
		static constexpr size_t sm_GpuAllocatorPageSize = 0x10000;   // 64K
		static constexpr size_t sm_CpuAllocatorPageSize = 0x200000;  // 2MB
		using FenceCheck = std::function<bool(uint64_t)>;
	public:
		LinearAllocator(ID3D12Device* device, AllocationPageType type, FenceCheck&& fence_check)
			: m_Device(device), m_Type(type) 
		{
			sm_PageManager[static_cast<size_t>(m_Type)].UpdateAvailablePages(std::forward<FenceCheck>(fence_check));
		}

		Allocation Allocate(size_t size, size_t alignment = 256);

		// Discard all page already in effect with fence value
		void SetFence(uint64_t fence);
	private:

		static size_t GetDefaultSize(AllocationPageType type) 
		{
			if (type == AllocationPageType::GPU_EXCLUSIVE) return sm_GpuAllocatorPageSize;
			if (type == AllocationPageType::CPU_WRITABLE)
				return sm_CpuAllocatorPageSize;
			else
				throw std::runtime_error("Unkown page type.");
			return 0;
		}
	private:
		static std::array<PageManager, static_cast<size_t>(AllocationPageType::NUM_TYPES)> sm_PageManager;

		ID3D12Device* m_Device;
		AllocationPageType m_Type;
		std::shared_ptr<LinearAllocationPage> m_CurrPage = nullptr;
		std::vector<std::shared_ptr<LinearAllocationPage>> m_Pages;
		std::vector<std::shared_ptr<LinearAllocationPage>> m_LargePages;
	};
}