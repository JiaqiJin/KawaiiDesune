#pragma once

#include <queue>

namespace RHI
{
	class CommandContext;
	class CommandListManager;

	class GraphicsContext;
	class ComputeContext;

	// Assign/destroy CommandContext
	class ContextManager
	{
	public:
		ContextManager() {}

		CommandContext* AllocateContext(D3D12_COMMAND_LIST_TYPE type);
		void FreeContext(CommandContext*);
		void DestroyAllContexts();
	private:
		std::vector<std::unique_ptr<CommandContext>> m_ContextPool[4];
		std::queue<CommandContext*> m_AvailableContexts[4];
		std::mutex m_ContextAllocationMutex;
	};

	class CommandContext
	{
		friend ContextManager;
	public:
		CommandContext() = default;
		CommandContext(const CommandContext&) = delete;
		CommandContext& operator=(const CommandContext&) = delete;

		void Initialize(ID3D12Device* pDevice);
	private:
		CommandContext(D3D12_COMMAND_LIST_TYPE type);
		void Reset();
	protected:
		ID3D12Device* m_Device;

		D3D12_COMMAND_LIST_TYPE m_Type;
	};
}