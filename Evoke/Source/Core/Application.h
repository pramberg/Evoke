#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Renderer\GraphicsContext.h"
#include "Renderer\RenderTarget.h"
#include "Renderer\EditorCameraController.h"

#include "Debug/ImGuiLayer.h"

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

		// #TEMP
		TSharedPtr<EditorCameraController>& CameraController() { return mCameraController; }
		TSharedPtr<RenderTarget2D>& RenderTarget() { return mAppRT; }
		void AddOnImGuiRenderCallback(const std::function<void()>& inFunction) { if (mImGuiLayer) { mImGuiLayer->OnRender.Subscribe(inFunction); } }

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

		ImGuiLayer* mImGuiLayer = nullptr;

		f32 mLastFrameTime = 0.0f;
		TUniquePtr<GraphicsContext> mContext;

		TSharedPtr<EditorCameraController> mCameraController;
		TSharedPtr<RenderTarget2D> mAppRT;

	private:
		static Application* sApplication;
	};

	Application* CreateApplication();
}