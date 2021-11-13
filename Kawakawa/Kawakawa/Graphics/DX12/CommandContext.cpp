#include "D3Dpch.h"
#include "CommandContext.h"
#include "DX12DriverAPI.h"
#include "CommandListManager.h"

namespace Kawaii::Graphics::backend::DX12
{
	//CommandContext* ContextManager::AllocateContext(D3D12_COMMAND_LIST_TYPE Type)
	//{
	//	auto& AvailableContexts = m_AvailableContexts[Type];

	//	CommandContext* context = nullptr;
	//	if (AvailableContexts.empty())
	//	{
	//		context = new CommandContext(Type);
	//		m_ContextPool[Type].emplace_back(context);
	//		context->Initialize();
	//	}
	//	else
	//	{
	//		context = AvailableContexts.front();
	//		AvailableContexts.pop();
	//		context->Reset();
	//	}
	//	assert(context != nullptr);

	//	assert(context->m_Type == Type);

	//	return context;
	//}

	//void ContextManager::FreeContext(CommandContext* UsedContext)
	//{
	//	assert(UsedContext != nullptr);
	//	m_AvailableContexts[UsedContext->m_Type].push(UsedContext);
	//}

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
	}

	void CommandContext::Initialize()
	{
		
	}

	void CommandContext::Reset()
	{

	}
}