#pragma once

#include <cassert>

inline size_t align(size_t x, size_t a)
{
    assert(((a - 1) & a) == 0 && "alignment is not a power of two");
    return (x + a - 1) & ~(a - 1);
}