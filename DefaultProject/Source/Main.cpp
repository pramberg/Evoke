#include <Evoke.h>
#include "ECS\Components.h"

using namespace Evoke;

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