#include "PCH.h"
#include "WindowsWindow.h"
#include <glad\glad.h>

namespace Evoke
{
	static bool sGLFWInitialized = false;

	TUniquePtr<Window> Window::Create(const WindowProperties& inProperties /*= WindowProperties()*/)
	{
		return MakeUnique<WindowsWindow>(inProperties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& inProperties) : mWindow(nullptr)
	{
		Init(inProperties);
	}

	WindowsWindow::~WindowsWindow()
	{
		Close();
	}

	void WindowsWindow::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(mWindow);
	}

	void WindowsWindow::SetVSync(b8 inEnabled)
	{
		if (inEnabled == GetVSyncEnabled())
			return;

		glfwSwapInterval(static_cast<i32>(inEnabled));

		mData.VSyncEnabled = inEnabled;
	}

	void WindowsWindow::Init(const WindowProperties& inProperties)
	{
		mData.Title = inProperties.Title;
		mData.Width = inProperties.Width;
		mData.Height = inProperties.Height;

		if (!sGLFWInitialized)
		{
			i32 success = glfwInit();
			EV_CORE_ASSERT(success, "Failed to initialize GLFW.");

			glfwSetErrorCallback([](i32 inError, const c8* inDescription)
			{
				EV_LOG(LogEngine, EV_ERROR, "GLFW Error ({}): {}", inError, inDescription);
			});

			sGLFWInitialized = true;
		}

		mWindow = glfwCreateWindow(static_cast<i32>(mData.Width), static_cast<i32>(mData.Height), mData.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(mWindow);

		i32 status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EV_CORE_ASSERT(status, "Failed to initialize Glad");

		glfwSetWindowUserPointer(mWindow, this);
		SetVSync(true);
		
		// Setup callbacks
		glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* inWindow, i32 inWidth, i32 inHeight)
		{
			WindowsWindow* window = reinterpret_cast<WindowsWindow*>(glfwGetWindowUserPointer(inWindow));

			window->mData.Width = inWidth;
			window->mData.Height = inHeight;
			window->OnWindowResized.Broadcast(inWidth, inHeight);
		});

		glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* inWindow)
		{
			WindowsWindow* window = reinterpret_cast<WindowsWindow*>(glfwGetWindowUserPointer(inWindow));
			window->OnWindowClosed.Broadcast();
		});

		glfwSetKeyCallback(mWindow, [](GLFWwindow* inWindow, i32 inKey, i32 inScanCode, i32 inAction, i32 inMods)
		{
			WindowsWindow* window = reinterpret_cast<WindowsWindow*>(glfwGetWindowUserPointer(inWindow));
			const EKeyCode keyCode = static_cast<EKeyCode>(inKey); // EKeyCode == GLFW key codes

			switch (inAction)
			{
			case GLFW_PRESS:
				window->OnKeyPressed.Broadcast(keyCode, 0);
				break;
			case GLFW_RELEASE:
				window->OnKeyReleased.Broadcast(keyCode);
				break;
			case GLFW_REPEAT:
				window->OnKeyPressed.Broadcast(keyCode, 1);
				break;
			}
		});

		glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* inWindow, i32 inKey, i32 inAction, i32 inMods)
		{
			WindowsWindow* window = reinterpret_cast<WindowsWindow*>(glfwGetWindowUserPointer(inWindow));
			const EMouseButton keyCode = static_cast<EMouseButton>(inKey); // EMouseButton == GLFW button codes

			switch (inAction)
			{
			case GLFW_PRESS:
				window->OnMouseButtonPressed.Broadcast(keyCode, 0);
				break;
			case GLFW_RELEASE:
				window->OnMouseButtonReleased.Broadcast(keyCode);
				break;
			}
		});

		glfwSetScrollCallback(mWindow, [](GLFWwindow* inWindow, f64 inXOffset, f64 inYOffset) 
		{
			WindowsWindow* window = reinterpret_cast<WindowsWindow*>(glfwGetWindowUserPointer(inWindow));
			window->OnMouseScrolled.Broadcast(static_cast<f32>(inXOffset), static_cast<f32>(inYOffset));
		});

		glfwSetCursorPosCallback(mWindow, [](GLFWwindow* inWindow, f64 inXPosition, f64 inYPosition)
		{
			WindowsWindow* window = reinterpret_cast<WindowsWindow*>(glfwGetWindowUserPointer(inWindow));
			window->OnMouseMoved.Broadcast(static_cast<f32>(inXPosition), static_cast<f32>(inYPosition));
		});
	}

	void WindowsWindow::Close()
	{
		glfwDestroyWindow(mWindow);
	}

}
