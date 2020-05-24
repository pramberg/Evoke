#include "PCH.h"
#include "Application.h"

extern Evoke::Application* Evoke::CreateApplication();

#ifdef EV_PLATFORM_WINDOWS

int main(i32 inArgCount, c8* inArgValues[])
{
	Evoke::Log::Init();

	auto app = Evoke::CreateApplication();
	app->Run();
	delete app;
}

#endif