#pragma once
#include "Core/Core.h"

namespace Evoke
{
	class TextureUtilities
	{
	public:
		template<typename T, std::enable_if_t<std::is_integral_v<T>>* _ = nullptr>
		static inline constexpr b8 IsPowerOfTwo(T inValue)
		{
			return (inValue != 0) && ((inValue & (inValue - 1)) == 0);
		}

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* _ = nullptr>
		static inline constexpr b8 IsPowerOfTwo(T inValue)
		{
			auto val = static_cast<u64>(inValue);
			return (val != 0) && ((val & (val - 1)) == 0);
		}

		static inline u8 CalculateNumMips(u16 inWidth, u16 inHeight, u16 inDepth = 1)
		{
			if (!IsPowerOfTwo(inWidth) || !IsPowerOfTwo(inHeight))
				return 1;
			const u16 max = std::max(std::max(inWidth, inHeight), inDepth);
			return 1 + u8(std::log2(max));
		}
	};
}