#pragma once
#include "Core/Window.h"

#include <GLFW/glfw3.h>

namespace Evoke
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& inProperties);
		virtual ~WindowsWindow();
	
		virtual void Update(f32 inDeltaTime) override;
	
		virtual u32 Width() const override { return mData.Width; }
		virtual u32 Height() const override { return mData.Height; }
	
		virtual void SetVSync(b8 inEnabled) override;
		virtual b8 VSyncEnabled() const override { return mData.VSyncEnabled; }

		virtual void* NativeWindow() const override { return mWindow; };
	
		virtual void SetCursorMode(ECursorMode inCursorMode) override;

	private:
		virtual void Init(const WindowProperties& inProperties);
		virtual void Close();

	private:
		GLFWwindow* mWindow;

		struct WindowData
		{
			string Title = "Window";
			u32 Width = 720;
			u32 Height = 1280;
			b8 VSyncEnabled = true;
		} mData;
	};
}