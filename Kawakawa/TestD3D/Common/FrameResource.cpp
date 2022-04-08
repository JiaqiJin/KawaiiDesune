#include "FrameResource.h"

FrameResource::FrameResource(ID3D12Device* device, UINT passCount, /*UINT objectCount*/ UINT maxInstanceCount, UINT materialCount)
{
    ThrowIfFailed(device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(CmdListAlloc.GetAddressOf())));

    //  FrameCB = std::make_unique<UploadBuffer<FrameConstants>>(device, 1, true);

}

FrameResource::~FrameResource()
{

}