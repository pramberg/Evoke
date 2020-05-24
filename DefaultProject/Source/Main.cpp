#include <Evoke.h>

class Sandbox : public Evoke::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}
};

Evoke::Application* Evoke::CreateApplication()
{
	return new Sandbox();
}