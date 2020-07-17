#include "PCH.h"
#include "Core/Input.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace Evoke
{
	b8 Input::IsKeyPressed(EKeyCode inKeycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Instance().MainWindow().NativeWindow());
		auto state = glfwGetKey(window, static_cast<i32>(inKeycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	b8 Input::IsMouseButtonPressed(EMouseButton inButton)
	{
		auto window = static_cast<GLFWwindow*>(Application::Instance().MainWindow().NativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<i32>(inButton));
		return state == GLFW_PRESS;
	}

	std::pair<f32, f32> Input::MousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Instance().MainWindow().NativeWindow());
		f64 x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (f32)x, (f32)y };
	}

	f32 Input::MouseX()
	{
		auto [x, y] = MousePosition();
		return x;
	}

	f32 Input::MouseY()
	{
		auto [x, y] = MousePosition();
		return y;
	}
}

