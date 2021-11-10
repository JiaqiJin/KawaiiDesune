#pragma once

#include "CommandQueue.h"
#include "../Utils/Singleton.h"

namespace RHI
{
	class CommandContext;

	// Compute Command List只支持这几种状态的过渡
#define VALID_COMPUTE_QUEUE_RESOURCE_STATES \
	    ( D3D12_RESOURCE_STATE_UNORDERED_ACCESS \
	    | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE \
		| D3D12_RESOURCE_STATE_COPY_DEST \
		| D3D12_RESOURCE_STATE_COPY_SOURCE )

	class ContextManager : public Singleton<ContextManager>
	{
	public:
		CommandContext* AllocateContext(D3D12_COMMAND_LIST_TYPE Type);
		void FreeContext(CommandContext* UsedContext);std::mutex sm_ContextAllocationMutex;
	private:
		std::vector<std::unique_ptr<CommandContext> > m_ContextPool[4];
		std::queue<CommandContext*> m_AvailableContexts[4];
		std::mutex sm_ContextAllocationMutex;
	};

	/**
	* CommandList和CommandListAllocator的集合体
	* 调用Static函数Begin会请求一个CommandContext，然后可以开始录制命令，调用End把命令推入CommandQueue
	* Begin时还会请求一个新的Allocator，End时回收这个Allocator
	* 每个线程使用自己的CommandContext
	*/
	class CommandContext
	{
		friend class ContextManager;
	private:
		CommandContext(D3D12_COMMAND_LIST_TYPE Type);

	public:
		CommandContext() = default;
		CommandContext(const CommandContext&) = delete;
		CommandContext& operator=(const CommandContext&) = delete;
		~CommandContext();

		// 创建CommandContext时调用，该函数会创建一个CommandList，并请求一个Allocator
		void Initialize();
		// 复用CommandContext时调用，重置渲染状态，该函数会请求一个Allocator，并调用CommandList::Reset
		void Reset();

	protected:	
		// Command List
		ID3D12GraphicsCommandList* m_CommandList;
		// Command Allcotor
		ID3D12CommandAllocator* m_CurrentAllocator;
		// Graphic rootSignature
		ID3D12RootSignature* m_CurGraphicsRootSignature;
		// Compute rootSignature
		ID3D12RootSignature* m_CurComputeRootSignature;
		// Pipeline State
		ID3D12PipelineState* m_CurPipelineState;

		// Resouce Barrier
		D3D12_RESOURCE_BARRIER m_ResourceBarrierBuffer[16];
		UINT m_NumBarriersToFlush;

		// Descriptor Heap
		ID3D12DescriptorHeap* m_CurrentDescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

		// Command type
		D3D12_COMMAND_LIST_TYPE m_Type;
	};
}