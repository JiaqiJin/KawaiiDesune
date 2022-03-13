#pragma once

namespace RHI
{
	class Graphics
	{
	public:
		Graphics() {}
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics() {  }
		//static members
		static ID3D12Device * s_Device;
	private:

	};
}