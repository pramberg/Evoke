#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Debug/ImGuiLayer.h"

namespace Evoke
{
	class AssetRegistry;
	class Scene;
	class EditorCameraController;
	class RenderTarget2D;
	class GraphicsContext;

	class Application
	{
	public:
		Application();
		Application(Application&) = delete;
		Application(Application&&) = delete;
		virtual ~Application();

		Application& operator=(Application&) = delete;

		void Run();
		void Close() { mIsRunning = false; }
		
		void PushLayer(Layer* inLayer);
		void PushOverlay(Layer* inLayer);

		Window& MainWindow() { return *mMainWindow; }
		const Window& MainWindow() const { return *mMainWindow; }

		// #TEMP
		inline TSharedPtr<EditorCameraController>& CameraController() { return mCameraController; }
		inline TSharedPtr<RenderTarget2D>& RenderTarget() { return mAppRT; }
		inline void AddOnImGuiRenderCallback(const std::function<void()>& inFunction) { if (mImGuiLayer) { mImGuiLayer->OnRender.Subscribe(inFunction); } }

		Scene* MainScene() { return mActiveScene.get(); }
		AssetRegistry* GetAssetRegistry() { return mAssetRegistry.get(); }
		void RenderScene(f32 inDeltaTime);
	public:
		static Application& Get() { return *sApplication; }

	private:
		void Update(f32 inDeltaTime);
		void OnWindowClose();
		void OnWindowResized(u32 inWidth, u32 inHeight);

	private:
		TUniquePtr<Window> mMainWindow;
		LayerStack mLayerStack;

		ImGuiLayer* mImGuiLayer = nullptr;

		TUniquePtr<GraphicsContext> mContext;

		TSharedPtr<EditorCameraController> mCameraController;
		TSharedPtr<RenderTarget2D> mAppRT;

		TUniquePtr<Scene> mActiveScene;
		f32 mLastFrameTime = 0.0f;
		b8 mIsRunning = true;

		TUniquePtr<AssetRegistry> mAssetRegistry;

		u8 mPadding[3];
	private:
		static Application* sApplication;
	};

	Application* CreateApplication();
}