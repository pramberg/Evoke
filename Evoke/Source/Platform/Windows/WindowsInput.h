#pragma once
#include "Core/Input.h"

namespace Evoke
{
	class WindowsInput : public Input
	{
		
	protected:
		virtual b8 IsKeyPressedImpl(i32 inKeycode) override;

		virtual b8 IsMouseButtonPressedImpl(i32 inButton) override;
		virtual std::pair<f32, f32> GetMousePositionImpl() override;
		virtual f32 GetMouseXImpl() override;
		virtual f32 GetMouseYImpl() override;
	};
}