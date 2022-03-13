#include "pch.h"
#include "CommandContext.h"
#include "Graphics.h"

namespace RHI
{
	CommandContext* ContextManager::AllocateContext(D3D12_COMMAND_LIST_TYPE type)
	{
		std::lock_guard<std::mutex> lockGuard(m_ContextAllocationMutex);

		auto& availableContexts = m_AvailableContexts[type];

		CommandContext* ret = nullptr;
		if (availableContexts.empty())
		{
			ret = new CommandContext(type);
			m_ContextPool[type].emplace_back(ret);
			ret->Initialize(Graphics::s_Device);
		}
		else
		{
			ret = availableContexts.front();
			availableContexts.pop();
			ret->Reset();
		}

		assert(ret != nullptr);
		assert(ret->m_Type == type);

		return ret;
	}

	void ContextManager::FreeContext(CommandContext* usedContext)
	{
		assert(usedContext != nullptr);
		std::lock_guard<std::mutex> lockGuard(m_ContextAllocationMutex);

		m_AvailableContexts[usedContext->m_Type].push(usedContext);
	}

	void ContextManager::DestroyAllContexts()
	{
		for (uint32_t i = 0; i < 4; ++i)
			m_ContextPool[i].clear();
	}

	void CommandContext::Initialize(ID3D12Device* pDevice)
	{

	}

	void CommandContext::Reset()
	{

	}
}