#pragma once
#include "Core.h"

namespace Evoke
{
	class Input
	{
	public:
		inline static b8 IsKeyPressed(i32 inKeycode) { return sInstance->IsKeyPressedImpl(inKeycode); }

		inline static b8 IsMouseButtonPressed(i32 inButton) { return sInstance->IsMouseButtonPressedImpl(inButton); }
		inline static std::pair<f32, f32> GetMousePosition() { return sInstance->GetMousePositionImpl(); }
		inline static f32 GetMouseX() { return sInstance->GetMouseXImpl(); }
		inline static f32 GetMouseY() { return sInstance->GetMouseYImpl(); }

	protected:
		virtual b8 IsKeyPressedImpl(i32 inKeycode) = 0;

		virtual b8 IsMouseButtonPressedImpl(i32 inButton) = 0;
		virtual std::pair<f32, f32> GetMousePositionImpl() = 0;
		virtual f32 GetMouseXImpl() = 0;
		virtual f32 GetMouseYImpl() = 0;

	private:
		static Input* sInstance;
	};
}