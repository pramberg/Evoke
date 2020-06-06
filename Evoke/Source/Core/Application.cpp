#include "PCH.h"
#include "Application.h"
#include "Window.h"

namespace Evoke
{
	Application::Application() : mMainWindow(std::unique_ptr<Window>(Window::Create()))
	{
		mMainWindow->OnWindowClosed.Subscribe([this]() { mIsRunning = false; });
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