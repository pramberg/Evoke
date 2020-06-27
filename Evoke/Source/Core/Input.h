#pragma once
#include "Core.h"

namespace Evoke
{
	class Input
	{
	public:
		static b8 IsKeyPressed(EKeyCode inKeycode);
		static b8 IsMouseButtonPressed(EMouseButton inButton);
		static std::pair<f32, f32> GetMousePosition();
		static f32 GetMouseX();
		static f32 GetMouseY();
	};
}