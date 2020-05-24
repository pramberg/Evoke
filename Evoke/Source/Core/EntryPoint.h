#include "PCH.h"
#include "Application.h"

extern Evoke::Application* Evoke::CreateApplication();

#ifdef EV_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Evoke::Log::Init();

	auto app = Evoke::CreateApplication();
	app->Run();
	delete app;
}

#endif