#include "PCH.h"
#include "WindowsInput.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace Evoke
{

	Input* Input::sInstance = new WindowsInput();

	b8 WindowsInput::IsKeyPressedImpl(i32 inKeycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, inKeycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	b8 WindowsInput::IsMouseButtonPressedImpl(i32 inButton)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, inButton);
		return state == GLFW_PRESS;
	}

	std::pair<f32, f32> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		f64 x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (f32)x, (f32)y };
	}

	f32 WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	f32 WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}

