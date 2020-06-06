#include "PCH.h"
#include "Application.h"
#include "Window.h"

namespace Evoke
{
	Application* Application::sApplication = nullptr;

	Application::Application() : mMainWindow(std::unique_ptr<Window>(Window::Create()))
	{
		EV_CORE_ASSERT(!sApplication, "Application already exists");
		sApplication = this;

		mMainWindow->OnWindowClosed.Subscribe([this]() { mIsRunning = false; });
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (mIsRunning)
		{
			for (Layer* layer : mLayerStack)
				layer->Update();

			mMainWindow->Update();
		}
	}

	void Application::PushLayer(Layer* inLayer)
	{
		mLayerStack.PushLayer(inLayer);
		inLayer->Attached();
	}

	void Application::PushOverlay(Layer* inLayer)
	{
		mLayerStack.PushOverlay(inLayer);
		inLayer->Attached();
	}

}