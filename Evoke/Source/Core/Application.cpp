#include "PCH.h"
#include "Application.h"
#include "Window.h"

namespace Evoke
{
	Application::Application() : mMainWindow(std::unique_ptr<Window>(Window::Create()))
	{
		mMainWindow->OnWindowClosed.Subscribe([this]() { mIsRunning = false; });
		mMainWindow->OnWindowResized.Subscribe([](u32 inWidth, u32 inHeight) { EV_CORE_TRACE("Resize({}, {})", inWidth, inHeight); });
		mMainWindow->OnMouseScrolled.Subscribe([](f32 inScrollX, f32 inScrollY) { EV_CORE_TRACE("Scroll({}, {})", inScrollX, inScrollY); });
		mMainWindow->OnKeyPressed.Subscribe([](i32 inKeyCode, i32 inRepeatCount) { EV_CORE_TRACE("KeyPressed({}, {})", inKeyCode, inRepeatCount); });
		mMainWindow->OnMouseMoved.Subscribe([](f32 inPositionX, f32 inPositionY) { EV_CORE_TRACE("MouseMoved({}, {})", inPositionX, inPositionY); });
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (mIsRunning)
		{
			mMainWindow->Update();
		}
	}
}