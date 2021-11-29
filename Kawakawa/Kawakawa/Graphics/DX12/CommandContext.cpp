#include "D3Dpch.h"
#include "CommandContext.h"
#include "DX12DriverAPI.h"
#include "Sampler.h"
#include "EnumConverter.h"

namespace Kawaii::Graphics::backend::DX12
{
	// ----------------- Command Context -------------------------
	// -----------------------------------------------------------
	CommandContext::CommandContext(DX12DriverAPI& driver, D3D12_COMMAND_LIST_TYPE Type)
		: m_Driver(driver), 
		m_Type(Type)
	{
		driver.GetCmdMgr().CreateNewCommandList(m_Type, &m_CommandList, &m_CommandAllocator);
	}

	CommandContext::~CommandContext()
	{
		if (m_CommandList) m_CommandList->Release();
		// Finish() is not called before destruction. so use the last completed fence
		if (m_CommandAllocator)
		{
			auto& queue = m_Driver.GetCmdMgr().GetQueue(m_Type);
			uint64_t fence_value = queue.GetLastCompletedFenceValue();
			queue.DiscardAllocator(fence_value, m_CommandAllocator);
		}
	}

	void CommandContext::Reset()
	{
		// We only call Reset() on previously freed contexts.  The command list persists, but we must
		// request a new allocator.
		assert(m_CommandList != nullptr && m_CommandAllocator == nullptr);
		m_CommandAllocator = m_Driver.GetCmdMgr().GetQueue(m_Type).RequestAllocator();
		m_CommandList->Reset(m_CommandAllocator, nullptr);

		m_NumBarriersToFlush = 0;
	}

	uint64_t CommandContext::Finish(bool wait_for_complete)
	{
		FlushResourceBarriers();

		CommandQueue& queue = m_Driver.GetCmdMgr().GetQueue(m_Type);

		uint64_t fence_value = queue.ExecuteCommandList(m_CommandList);
		// TODO

		queue.DiscardAllocator(fence_value, m_CommandAllocator);
		m_CommandAllocator = nullptr;


		if (wait_for_complete)
			m_Driver.GetCmdMgr().WaitForFence(fence_value);

		return fence_value;
	}

	void CommandContext::TransitionResource(GpuResource& resource, D3D12_RESOURCE_STATES new_state, bool flush_immediate)
	{
		D3D12_RESOURCE_STATES old_state = resource.m_UsageState;
		if (old_state != new_state)
		{
			assert(m_NumBarriersToFlush < 16 && "Exceeded arbitrary limit on buffered barriers");
			D3D12_RESOURCE_BARRIER& barrier_desc = m_Barriers[m_NumBarriersToFlush++];
			barrier_desc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier_desc.Transition.pResource = resource.GetResource();
			barrier_desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier_desc.Transition.StateBefore = old_state;
			barrier_desc.Transition.StateAfter = new_state;

			if (new_state == resource.m_TransitioningState) 
			{
				barrier_desc.Flags = D3D12_RESOURCE_BARRIER_FLAG_END_ONLY;
				resource.m_TransitioningState = static_cast<D3D12_RESOURCE_STATES>(-1);
			}
			else 
			{
				barrier_desc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			}
			resource.m_UsageState = new_state;
		}

		if (flush_immediate || m_NumBarriersToFlush == 16) 
			FlushResourceBarriers();
	}

	void CommandContext::FlushResourceBarriers()
	{
		if (m_NumBarriersToFlush > 0)
		{
			m_CommandList->ResourceBarrier(m_NumBarriersToFlush, m_Barriers.data());
			m_NumBarriersToFlush = 0;
		}
	}
}