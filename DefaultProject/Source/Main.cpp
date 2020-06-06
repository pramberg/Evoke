#include <Evoke.h>

class SceneLayer : public Evoke::Layer
{
public:
	SceneLayer() : Layer("Scene") {}

	virtual void Attached() override
	{
		auto& app = Evoke::Application::Get();
		app.MainWindow().OnKeyPressed.Subscribe([](i32 inKeyCode, i32 inRepeatCount) { EV_INFO("KeyPressed({}, {})", inKeyCode, inRepeatCount); });
	}

	virtual void Update() override
	{

	}

};

class Sandbox : public Evoke::Application
{
public:
	Sandbox()
	{
		PushLayer(new SceneLayer());
	}

	~Sandbox()
	{
	}
};

Evoke::Application* Evoke::CreateApplication()
{
	return new Sandbox();
}