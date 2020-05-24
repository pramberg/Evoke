#include <Evoke.h>

class Project : public Evoke::Application
{
public:
	Project()
	{

	}

	~Project()
	{

	}

	void Run()
	{
		Evoke::Log::GetCoreLogger()->warn("Test");
	}
};

Evoke::Application* Evoke::CreateApplication()
{
	return new Project();
}