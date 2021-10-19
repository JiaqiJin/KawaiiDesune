#pragma once

// A render pass consists of a subset of the commands that you record into a command list.
// Render passes can improve the performance of your renderer if it's based on Tile-Based Deferred Rendering
// improve GPU efficiency by reducing memory traffic to/from off-chip memory
namespace RHI
{
	struct rtvAttachemntDesc
	{
		D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorHandle;
		D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE rtvBeginningAccess;
		D3D12_RENDER_PASS_ENDING_ACCESS_TYPE rtvEndingAccess;
		D3D12_CLEAR_VALUE clearValue;
	};

	struct dsvAttachmentDesc
	{
		D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorHandle;
		D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE depthBeginningAccess;
		D3D12_RENDER_PASS_ENDING_ACCESS_TYPE depthEndingAccess;
		D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE stencilBeginningAccess = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_NO_ACCESS;
		D3D12_RENDER_PASS_ENDING_ACCESS_TYPE stencilEndingAccess = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_NO_ACCESS;
		D3D12_CLEAR_VALUE clearValue;
	};

	struct RenderPassDesc
	{
		std::vector<rtvAttachemntDesc> rtvAttachments;
		std::optional<dsvAttachmentDesc> dsvAttachment = std::nullopt;
		D3D12_RENDER_PASS_FLAGS renderPassFlags = D3D12_RENDER_PASS_FLAG_NONE;
		uint32_t width, height;
	};

	class RenderPass
	{
	public:
		RenderPass() = default;
		RenderPass(RenderPassDesc const& desc);

		void BeginRenderPass(ID3D12GraphicsCommandList4* cmdList);
		void EndRenderPass(ID3D12GraphicsCommandList4* cmdList);
	private:
		uint32_t width = 0, height = 0;
		std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> rtvs{};
		std::unique_ptr<D3D12_RENDER_PASS_DEPTH_STENCIL_DESC> dsv = nullptr;
		D3D12_RENDER_PASS_FLAGS flags = D3D12_RENDER_PASS_FLAG_NONE;
	};
}