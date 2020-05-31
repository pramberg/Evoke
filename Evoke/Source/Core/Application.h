#pragma once
#include "Core.h"
#include "Window.h"

namespace Evoke
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> mMainWindow;
		b8 mIsRunning = true;
	};

	Application* CreateApplication();
}