#pragma once

#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdint>
#include <memory>

#ifdef ALIGN
#undef ALIGN
#endif

#define ALIGN(x, a) (((x) + ((a)-1)) & ~((a)-1))

using four_char_enum = int32_t;

#define ENUM(e) enum class e : four_char_enum

namespace Kawaii
{
    /*
    * The use of macros is to unify the old enumeration definition syntax before C++11 and the new type-safe syntax afterwards. 
    * Using char[] to calculate the value is to know the meaning directly through the value when debugging the output.
    */
    namespace details {
        constexpr int32_t i32(const char* s, int32_t v) {
            return *s ? i32(s + 1, v * 256 + *s) : v;
        }

        constexpr uint16_t u16(const char* s, uint16_t v) {
            return *s ? u16(s + 1, v * 256 + *s) : v;
        }

        constexpr uint32_t u32(const char* s, uint32_t v) {
            return *s ? u32(s + 1, v * 256 + *s) : v;
        }
    }  // namespace details

    constexpr int32_t operator"" _i32(const char* s, size_t) {
        return details::i32(s, 0);
    }

    constexpr uint32_t operator"" _u32(const char* s, size_t) {
        return details::u32(s, 0);
    }

    constexpr uint16_t operator"" _u16(const char* s, size_t) {
        return details::u16(s, 0);
    }
}

