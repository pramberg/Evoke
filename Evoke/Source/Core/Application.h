#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Renderer\GraphicsContext.h"

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

		Window& MainWindow() { return *mMainWindow; }
		const Window& MainWindow() const { return *mMainWindow; }

	public:
		static Application& Instance() { return *sApplication; }

	private:
		void Update(f32 inDeltaTime);
		void OnWindowClose();
		void OnWindowResized(u32 inWidth, u32 inHeight);

	private:
		TUniquePtr<Window> mMainWindow;
		b8 mIsRunning = true;
		LayerStack mLayerStack;
		f32 mLastFrameTime = 0.0f;
		TUniquePtr<GraphicsContext> mContext;

	private:
		static Application* sApplication;
	};

	Application* CreateApplication();
}