#pragma once

#include "../../Utility/Singleton.h"

namespace Kawaii::Graphics::backend::DX12
{
	class CommandContext;
	class GraphicsContext;
	class ComputeContext;
	class DX12DriverAPI;
	// Compute Command List只支持这几种状态的过渡
#define VALID_COMPUTE_QUEUE_RESOURCE_STATES \
	    ( D3D12_RESOURCE_STATE_UNORDERED_ACCESS \
	    | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE \
		| D3D12_RESOURCE_STATE_COPY_DEST \
		| D3D12_RESOURCE_STATE_COPY_SOURCE )

	//class ContextManager : public Singleton<ContextManager>
	//{
	//public:
	//	CommandContext* AllocateContext(D3D12_COMMAND_LIST_TYPE Type);
	//	void FreeContext(CommandContext* UsedContext);

	//private:
	//	std::vector<std::unique_ptr<CommandContext> > m_ContextPool[4];
	//	std::queue<CommandContext*> m_AvailableContexts[4];
	//};

	/**
	* CommandList和CommandListAllocator的集合体
	* 调用Static函数Begin会请求一个CommandContext，然后可以开始录制命令，调用End把命令推入CommandQueue
	* Begin时还会请求一个新的Allocator，End时回收这个Allocator
	* 每个线程使用自己的CommandContext
	*/
	class CommandContext
	{
	private:
		CommandContext(DX12DriverAPI& driver, D3D12_COMMAND_LIST_TYPE Type);
		// 创建CommandContext时调用，该函数会创建一个CommandList，并请求一个Allocator
		void Initialize();
		// 复用CommandContext时调用，重置渲染状态，该函数会请求一个Allocator，并调用CommandList::Reset
		void Reset();

	public:
		~CommandContext();

	private:
		DX12DriverAPI& m_Driver;
		// Command List Type
		D3D12_COMMAND_LIST_TYPE m_Type;
		// Command List
		ID3D12GraphicsCommandList5* m_CommandList = nullptr;
		// Command Allocator
		ID3D12CommandAllocator* m_CommandAllocator = nullptr;

		// Descriptor Heaps
		std::array<ID3D12DescriptorHeap*, D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES> m_CurrentDescriptorHeaps{};
		// Root Signature
		ID3D12RootSignature* m_RootSignature;
	};
}