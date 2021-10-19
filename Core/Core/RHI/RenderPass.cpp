#include "../Kawaiipch.h"
#include "RenderPass.h"

namespace RHI
{
	RenderPass::RenderPass(RenderPassDesc const& desc)
	{
		for (auto& const attachment : desc.rtvAttachments)
		{
			D3D12_RENDER_PASS_RENDER_TARGET_DESC rtvDesc = {};
			rtvDesc.cpuDescriptor = attachment.CPUDescriptorHandle;
			rtvDesc.BeginningAccess = { attachment.rtvBeginningAccess, attachment.clearValue };
			rtvDesc.EndingAccess = { attachment.rtvEndingAccess, {} }; 

			rtvs.push_back(rtvDesc);
		}

		if(desc.dsvAttachment)
		{
			auto const& dsvAttach = desc.dsvAttachment.value();

			D3D12_RENDER_PASS_DEPTH_STENCIL_DESC dsvDesc = {};
			dsvDesc.cpuDescriptor = dsvAttach.CPUDescriptorHandle;
			dsvDesc.DepthBeginningAccess = { dsvAttach.depthBeginningAccess, dsvAttach.clearValue };
			dsvDesc.StencilBeginningAccess = { dsvAttach.stencilBeginningAccess, dsvAttach.clearValue };
			dsvDesc.DepthEndingAccess = { dsvAttach.depthEndingAccess, {} };
			dsvDesc.StencilEndingAccess = { dsvAttach.stencilEndingAccess, {} };

			dsv = std::make_unique<D3D12_RENDER_PASS_DEPTH_STENCIL_DESC>(dsvDesc);
		}
	}

	void RenderPass::BeginRenderPass(ID3D12GraphicsCommandList4* cmdList)
	{
		cmdList->BeginRenderPass(static_cast<uint32_t>(rtvs.size()), rtvs.data(), dsv.get(), flags);

		D3D12_VIEWPORT vp = {};
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		cmdList->RSSetViewports(1, &vp);
		D3D12_RECT rect = {};
		rect.bottom = (int64_t)height;
		rect.left = 0;
		rect.right = (int64_t)width;
		rect.top = 0;
		cmdList->RSSetScissorRects(1, &rect);
	}

	void RenderPass::EndRenderPass(ID3D12GraphicsCommandList4* cmdList)
	{
		cmdList->EndRenderPass();
	}
}