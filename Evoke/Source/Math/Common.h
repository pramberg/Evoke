#pragma once
#include "Core/Core.h"

namespace Evoke
{
	namespace Math
	{
		constexpr f64 Pi = 3.14159265358979323846;
		constexpr f64 TwoPi = Pi * 2.0f;

		template<typename TA, typename TB, typename TAlpha>
		constexpr auto Lerp(const TA& inA, const TB& inB, const TAlpha& inAlpha)
		{
			const auto s = 1.0f - inAlpha;
			return inA * s + inB * inAlpha;
		}
	};
}