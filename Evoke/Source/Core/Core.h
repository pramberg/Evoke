#pragma once
#include <stdint.h>
#include <string>
#include <type_traits>
#include <memory>

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

//using string = std::string;
using wstring = std::wstring;
using cstring = const c8*;

using b8 = bool;