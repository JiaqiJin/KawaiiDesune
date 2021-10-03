#pragma once

namespace Kawaii
{
	class Device;

	class CommandList
	{
	public:
		CommandList();
		~CommandList();

		CommandList(const CommandList&) = delete;
		CommandList(CommandList&&) = delete;
		CommandList& operator=(const CommandList&) = delete;
		CommandList& operator=(CommandList&&) = delete;

		ID3D12GraphicsCommandList6* GetCommandList() { return m_CommandList.Get(); }

	protected:
		bool Initialize(ID3D12GraphicsCommandList6* commandList);

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> m_CommandList;
	};

	// Graphic Command List
	class GraphicCommandList : public CommandList
	{
	public:
		GraphicCommandList() = default;
		~GraphicCommandList() = default;
		
		bool Initialize(Device& device, ID3D12CommandAllocator* allocator, ID3D12PipelineState* PSO);
		bool Reset(ID3D12CommandAllocator* allocator, ID3D12PipelineState* PSO);
	};

	// Copy Command List
	class CopyCommandList : public CommandList
	{
	public:
		CopyCommandList() = default;
		~CopyCommandList() = default;

		bool Initialize(Device & device, ID3D12CommandAllocator * allocator, ID3D12PipelineState * PSO);
		bool Reset(ID3D12CommandAllocator * allocator, ID3D12PipelineState * PSO);
	};

	class ComputeCommandList : public CommandList
	{
	public:
		ComputeCommandList() = default;
		~ComputeCommandList() = default;

		bool Initialize(Device& device, ID3D12CommandAllocator* allocator, ID3D12PipelineState* PSO);
		bool Reset(ID3D12CommandAllocator* allocator, ID3D12PipelineState* PSO);
	};
}