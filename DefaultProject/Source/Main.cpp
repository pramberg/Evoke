#include <Evoke.h>

class SceneLayer : public Evoke::Layer
{
public:
	SceneLayer() : Layer("Scene") {}

	virtual void Attached() override
	{
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