#pragma once
#include "Core.h"

namespace Evoke
{
	class Input
	{
	public:
		static b8 IsKeyPressed(EKeyCode inKeycode);
		static b8 IsMouseButtonPressed(EMouseButton inButton);
		static std::pair<f32, f32> MousePosition();
		static f32 MouseX();
		static f32 MouseY();
	};
}