#pragma once

#include "DX12Resource.h"

namespace RHI
{
	class DX12Buffer
	{
	public:
		DX12Buffer() {}

		virtual ~DX12Buffer() {}

		DX12Resource* GetResource() { return m_ResourceAllocation.m_UnderlyingResource; }
		
	private:
		DX12ResourceAllocation m_ResourceAllocation;
	};
}