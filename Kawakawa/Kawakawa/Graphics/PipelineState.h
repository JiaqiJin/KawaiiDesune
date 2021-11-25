#pragma once

#include "Resource.h"

#include <string>
#include <optional>
#include <cassert>
#include <array>
#include <set>

namespace Kawaii::Graphics
{
	class DriverAPI;

	class PipelineState : public Resource
	{
	public:
		PipelineState(std::string_view name) : Resource(name, nullptr) {}

		inline const std::string& GetName() const noexcept { return m_Name; }
	private:

	};
	
}