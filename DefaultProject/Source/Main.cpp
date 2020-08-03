#include <Evoke.h>

using namespace Evoke;

class SceneLayer : public Evoke::Layer
{
public:
	SceneLayer() : Layer("Scene") {}

	virtual void Attached() override
	{
	}

	virtual void Update(f32 inDeltaTime) override
	{
		mActiveScene->Update(inDeltaTime);
	}

private:
	TUniquePtr<Scene> mActiveScene;
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