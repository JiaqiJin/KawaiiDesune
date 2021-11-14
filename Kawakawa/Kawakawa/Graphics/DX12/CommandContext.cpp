#include "D3Dpch.h"
#include "CommandContext.h"
#include "DX12DriverAPI.h"
#include "CommandListManager.h"

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
	}

	void CommandContext::Initialize()
	{
		
	}

	void CommandContext::Reset()
	{

	}
}