#pragma once
#include "PCH.h"

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;
using f128 = long double;

using c8 = char;
using c16 = char16_t;
using c32 = char32_t;

using string = std::string;

using b8 = bool;

#define EV_ENABLE_ASSERTS
#ifdef EV_ENABLE_ASSERTS
#include <Debug/DebugBreak.h>
#define EV_ASSERT(x, ...) { if(!(x)) { EV_ERROR("Assertion failed: {}", __VA_ARGS__); psnip_trap(); } }
#define EV_CORE_ASSERT(x, ...) { if(!(x)) { EV_CORE_ERROR("Assertion failed: {}", __VA_ARGS__); psnip_trap(); } }
#else
#define EV_ASSERT(x, ...)
#define EV_CORE_ASSERT(x, ...)
#endif // EV_ENABLE_ASSERTS
