#pragma once
#include "Core.h"

namespace Evoke
{
	class Input
	{
	public:
		inline static b8 IsKeyPressed(i32 inKeycode);
		inline static b8 IsMouseButtonPressed(i32 inButton);
		inline static std::pair<f32, f32> GetMousePosition();
		inline static f32 GetMouseX();
		inline static f32 GetMouseY();
	};
}