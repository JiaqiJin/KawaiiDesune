#pragma once

#include <cstdint>

namespace Util
{
	inline constexpr uint64_t AlignToPowerOfTwo(uint64_t address, uint64_t align)
	{
		if ((0 == align) || (align & (align - 1))) return address;

		return ((address + (align - 1)) & ~(align - 1));
	}

	inline constexpr uint64_t Align(uint64_t address, uint64_t align)
	{
		if (align == 0 || align == 1) return address;
		uint64_t r = address % align;
		return r ? address + (align - r) : address;
	}

	using OffsetType = size_t;

	inline constexpr OffsetType const INVALID_OFFSET = static_cast<OffsetType>(-1);
}