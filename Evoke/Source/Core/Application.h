#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

namespace Evoke
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		
		void PushLayer(Layer* inLayer);
		void PushOverlay(Layer* inOverlay);

		Window& MainWindow() { return *mMainWindow; }
		const Window& MainWindow() const { return *mMainWindow; }
		static Application& Get() { return *sApplication; }
	private:
		std::unique_ptr<Window> mMainWindow;
		b8 mIsRunning = true;
		LayerStack mLayerStack;

	private:
		static Application* sApplication;
	};

	Application* CreateApplication();
}