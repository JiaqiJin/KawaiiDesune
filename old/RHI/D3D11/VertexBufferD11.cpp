#include "VertexBufferD11.h"
#include "../GraphicsMgr.h"
#include "GraphicsMgrD11.h"
#include "../../Platform/Application.h"

#include <d3d11.h>
using namespace Excalibur;

Excalibur::VertexBufferD11::VertexBufferD11(void * data, unsigned int count, VertexFormat vf)
{
	Initialize(data, count, vf);
}

Excalibur::VertexBufferD11::~VertexBufferD11()
{
	//Finialize();
	if (mVertexBuffer) {
		mVertexBuffer->Release();
	}
}

void Excalibur::VertexBufferD11::Initialize(void * data, unsigned int count, VertexFormat vf)
{
	//VertexBufferBase::Initialize(data, count, vf);

	m_VertexCount = count;

	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = GetVertexSize(vf) * count;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = data;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	auto result = mgrd11->m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &(this->mVertexBuffer));
}
