#include "../Kawaiipch.h"
#include "CommandContext.h"
#include "CommandListManager.h"

namespace RHI
{
	CommandContext* ContextManager::AllocateContext(D3D12_COMMAND_LIST_TYPE Type)
	{
		std::lock_guard<std::mutex> LockGuard(sm_ContextAllocationMutex);

		auto& AvailableContexts = m_AvailableContexts[Type];

		CommandContext* context = nullptr;
		// 创建一个新的context
		if (AvailableContexts.empty())
		{
			context = new CommandContext(Type);
			m_ContextPool[Type].emplace_back(context);
			context->Initialize();
		}
		else
		{
			context = AvailableContexts.front();
			AvailableContexts.pop();
			context->Reset();
		}
		assert(context != nullptr);

		assert(context->m_Type == Type);

		return context;
	}

	void ContextManager::FreeContext(CommandContext* UsedContext)
	{
		assert(UsedContext != nullptr);
		std::lock_guard<std::mutex> LockGuard(sm_ContextAllocationMutex);
		m_AvailableContexts[UsedContext->m_Type].push(UsedContext);
	}

	// ------------------------- Command Context -------------------------------

	CommandContext::CommandContext(D3D12_COMMAND_LIST_TYPE Type) 
		: m_Type(Type)
	{
		m_CommandList = nullptr;
		m_CurrentAllocator = nullptr;
		ZeroMemory(m_CurrentDescriptorHeaps, sizeof(m_CurrentDescriptorHeaps));

		m_CurGraphicsRootSignature = nullptr;
		m_CurComputeRootSignature = nullptr;
		m_CurPipelineState = nullptr;
		m_NumBarriersToFlush = 0;
	}

	CommandContext::~CommandContext()
	{
		if (m_CommandList != nullptr)
			m_CommandList->Release();
	}

	void CommandContext::Initialize()
	{
		CommandListManager::GetSingleton().CreateNewCommandList(m_Type, &m_CommandList, &m_CurrentAllocator);
	}

	void CommandContext::Reset()
	{
		assert(m_CommandList != nullptr && m_CurrentAllocator == nullptr);

		m_CurrentAllocator = CommandListManager::GetSingleton().GetQueue(m_Type).RequestAllocator();
		m_CommandList->Reset(m_CurrentAllocator, nullptr);

		m_CurGraphicsRootSignature = nullptr;
		m_CurComputeRootSignature = nullptr;
		m_CurPipelineState = nullptr;
		m_NumBarriersToFlush = 0;

		// TODO
	}
}