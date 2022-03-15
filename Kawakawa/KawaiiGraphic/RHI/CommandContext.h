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

		// Prepare to render by reserving a command list and command allocator
		void Initialize(ID3D12Device* pDevice);

		GraphicsContext& GetGraphicsContext()
		{
			ASSERT(m_Type != D3D12_COMMAND_LIST_TYPE_COMPUTE, "Cannot convert async compute context to graphics");
			return reinterpret_cast<GraphicsContext&>(*this);
		}

		ComputeContext& GetComputeContext()
		{
			return reinterpret_cast<ComputeContext&>(*this);
		}
	private:
		CommandContext(D3D12_COMMAND_LIST_TYPE type);
		void Reset();
	protected:
		ID3D12Device* m_Device;

		D3D12_COMMAND_LIST_TYPE m_Type;
	};

	class GraphicsContext : public CommandContext
	{

	};

	class ComputeContext : public CommandContext
	{

	};
}