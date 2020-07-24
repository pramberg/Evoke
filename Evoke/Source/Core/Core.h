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
using string_view = std::string_view;
using String = std::string;
using StringView = std::string_view;

using b8 = bool;

#define EV_ENABLE_ASSERTS
#ifdef EV_ENABLE_ASSERTS
#include <Debug/DebugBreak.h>
#define EV_ASSERT(x, ...) { if(!(x)) { EV_LOG(LogApp, EV_ERROR, "Assertion failed: {}", __VA_ARGS__); psnip_trap(); } }
#define EV_CORE_ASSERT(x, ...) { if(!(x)) { EV_LOG(LogEngine, EV_ERROR, "Assertion failed: {}", __VA_ARGS__); psnip_trap(); } }
#else
#define EV_ASSERT(x, ...)
#define EV_CORE_ASSERT(x, ...)
#endif // EV_ENABLE_ASSERTS

#define EV_BIND_0(inFn) std::bind(&inFn, this)
#define EV_BIND_1(inFn) std::bind(&inFn, this, std::placeholders::_1)
#define EV_BIND_2(inFn) std::bind(&inFn, this, std::placeholders::_1, std::placeholders::_2)
#define EV_BIND_3(inFn) std::bind(&inFn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define EV_BIND_4(inFn) std::bind(&inFn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

namespace Evoke
{
	template<typename T>
	using TSharedPtr = std::shared_ptr<T>;

	template<typename T, typename... TArgs>
	constexpr TSharedPtr<T> MakeShared(TArgs&&... inArgs)
	{
		return std::make_shared<T>(std::forward<TArgs>(inArgs)...);
	}

	template<typename T>
	using TUniquePtr = std::unique_ptr<T>;

	template<typename T, typename... TArgs>
	constexpr TUniquePtr<T> MakeUnique(TArgs&&... inArgs)
	{
		return std::make_unique<T>(std::forward<TArgs>(inArgs)...);
	}
}

#include "Math/Common.h"
#include "Math/Types.h"