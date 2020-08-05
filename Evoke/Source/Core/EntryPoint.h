#include "PCH.h"
#include "Application.h"
#include "CommandLine.h"
#include "Platform/RenderDoc/RenderDoc.h"

extern Evoke::Application* Evoke::CreateApplication();

#ifdef EV_PLATFORM_WINDOWS

i32 main(i32 inArgCount, c8* inArgValues[])
{
	Evoke::CommandLine::Init(inArgCount, inArgValues);
	Evoke::Log::Init();
	if (Evoke::CommandLine::HasParameter("-RD"))
	{
		// This should come from preferences in the future.
		const String pathToRenderDoc = Evoke::CommandLine::Arguments("-RD").value()[0];
		Evoke::RenderDoc::TryInject(pathToRenderDoc);
	}

	auto app = Evoke::CreateApplication();
	app->Run();
	delete app;

	Evoke::RenderDoc::Close();
}

#endif