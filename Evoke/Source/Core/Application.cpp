#include "PCH.h"
#include "Application.h"
#include "Window.h"
#include "Debug/ImGuiLayer.h"
#include "glad/glad.h"

namespace Evoke
{
	Application* Application::sApplication = nullptr;

	Application::Application() : mMainWindow(std::unique_ptr<Window>(Window::Create()))
	{
		EV_CORE_ASSERT(!sApplication, "Application already exists");
		sApplication = this;

		mMainWindow->OnWindowClosed.Subscribe(EV_BIND_0(Application::OnWindowClose));
		mMainWindow->OnWindowResized.Subscribe(EV_BIND_2(Application::OnWindowResized));
		PushOverlay(new ImGuiLayer());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (mIsRunning)
		{
			Update();
		}
	}

	void Application::Update()
	{
		for (Layer* layer : mLayerStack)
			layer->Update();

		mMainWindow->Update();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Application::OnWindowClose()
	{
		Close();
	}

	void Application::OnWindowResized(u32 inWidth, u32 inHeight)
	{
		Update();
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