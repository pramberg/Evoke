#pragma once
#include "Core/Core.h"

namespace Evoke
{
	namespace Math
	{
		template<typename T = f64> constexpr T Pi = static_cast<T>(3.14159265358979323846);
		template<typename T = f64> constexpr T TwoPi = Pi<T> * 2.0;
		template<typename T = f64> constexpr T HalfPi = Pi<T> / 2.0;

		template<typename TA, typename TB, typename TAlpha>
		constexpr auto Lerp(const TA& inA, const TB& inB, const TAlpha& inAlpha)
		{
			const auto s = 1.0f - inAlpha;
			return inA * s + inB * inAlpha;
		}
	};
}