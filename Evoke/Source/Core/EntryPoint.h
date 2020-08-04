#include "PCH.h"
#include "Application.h"
#include "CommandLine.h"

extern Evoke::Application* Evoke::CreateApplication();

#ifdef EV_PLATFORM_WINDOWS

i32 main(i32 inArgCount, c8* inArgValues[])
{
	Evoke::CommandLine::Init(inArgCount, inArgValues);
	Evoke::Log::Init();

	auto app = Evoke::CreateApplication();
	app->Run();
	delete app;
}

#endif