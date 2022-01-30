#pragma once

#include "../SamplerState.h"
#include <d3d11.h>

namespace Excalibur {

	class SamplerStateD11 : public SamplerState
	{
	public:
		SamplerStateD11();
		virtual ~SamplerStateD11();
		virtual void Initialize();
		virtual void Finialize();
	public:

		ID3D11SamplerState* m_sampleState;
	};
}