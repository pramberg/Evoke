#include "PCH.h"
#include "Core/Input.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace Evoke
{
	b8 Input::IsKeyPressed(EKeyCode inKeycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<i32>(inKeycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	b8 Input::IsMouseButtonPressed(EMouseButton inButton)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<i32>(inButton));
		return state == GLFW_PRESS;
	}

	std::pair<f32, f32> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		f64 x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (f32)x, (f32)y };
	}

	f32 Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	f32 Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}

