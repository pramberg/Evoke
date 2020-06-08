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
		void Close() { mIsRunning = false; }
		
		void PushLayer(Layer* inLayer);
		void PushOverlay(Layer* inLayer);

		Window& GetWindow() { return *mMainWindow; }
		const Window& GetWindow() const { return *mMainWindow; }

	public:
		static Application& Get() { return *sApplication; }

	private:
		void Update();
		void OnWindowClose();
		void OnWindowResized(u32 inWidth, u32 inHeight);

	private:
		std::unique_ptr<Window> mMainWindow;
		b8 mIsRunning = true;
		LayerStack mLayerStack;

	private:
		static Application* sApplication;
	};

	Application* CreateApplication();
}