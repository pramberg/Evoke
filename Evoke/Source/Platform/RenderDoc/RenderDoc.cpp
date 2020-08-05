#include "PCH.h"
#include "RenderDoc.h"
#include "Platform/Generic/Filesystem.h"

#include "dynalo/dynalo.hpp"

namespace Evoke
{
#ifdef EV_PLATFORM_WINDOWS
	i32 CALLBACK EnumWindowsProc(HWND inHandle, LPARAM inLParam)
	{
		DWORD targetID = (DWORD)inLParam;
		DWORD currentID = 0;
		::GetWindowThreadProcessId(inHandle, &currentID);
		if (targetID == currentID)
		{
			::SetForegroundWindow(inHandle);
			::SetFocus(inHandle);
			::SetActiveWindow(inHandle);
			
			WINDOWPLACEMENT windowPlacement;
			windowPlacement.length = sizeof(WINDOWPLACEMENT);
			::GetWindowPlacement(inHandle, &windowPlacement);
			if (windowPlacement.showCmd == SW_MINIMIZE || windowPlacement.showCmd == SW_SHOWMINIMIZED || windowPlacement.showCmd == SW_SHOWNORMAL)
				::ShowWindow(inHandle, SW_RESTORE);
			
			return false;
		}
		return true;
	}
#endif

	void BringWindowToForeground(u64 inProcessID)
	{
#ifdef EV_PLATFORM_WINDOWS
		EnumWindows(&EnumWindowsProc, (LPARAM)inProcessID);
#else
		// #TODO: Implement on other platforms.
#endif
	}

	RENDERDOC_API_1_1_2* RenderDoc::sRenderDocAPI = nullptr;
	u32 RenderDoc::sRenderDocProcessID = 0;
	void* RenderDoc::sRenderDocHandle = nullptr;

	b8 RenderDoc::TryInject(StringView inPathToRenderDoc)
	{
		auto path = std::filesystem::path(inPathToRenderDoc);
		path.make_preferred();
		if (path.has_filename())
			path += Filesystem::Separator;
		path += dynalo::to_native_name("renderdoc");

		try { sRenderDocHandle = dynalo::open(path.string()); }
		catch (const std::runtime_error&) { return false; }

		using GetAPISignature = i32(RENDERDOC_Version, void**);

		const auto GetAPIFn = dynalo::get_function<GetAPISignature>((dynalo::native::handle)sRenderDocHandle, "RENDERDOC_GetAPI");
		const i32 result = GetAPIFn(eRENDERDOC_API_Version_1_1_2, (void**)&sRenderDocAPI);
		
		EV_CORE_ASSERT(result == 1, "Failed to initialize RenderDoc.");

		SetCaptureTemplate();
		return true;
	}

	void RenderDoc::Close()
	{
		if (sRenderDocHandle)
			dynalo::close((dynalo::native::handle)sRenderDocHandle);
	}

	void RenderDoc::LaunchUI(StringView inCommandLine)
	{
		if (!sRenderDocAPI->IsTargetControlConnected())
		{
			sRenderDocProcessID = sRenderDocAPI->LaunchReplayUI(1, inCommandLine.data());
		}
		else
		{
			BringWindowToForeground(sRenderDocProcessID);
		}
	}

	void RenderDoc::TriggerCapture()
	{
		sRenderDocAPI->TriggerCapture();
	}

	void RenderDoc::SetCaptureTemplate(StringView inTemplate)
	{
		sRenderDocAPI->SetCaptureFilePathTemplate(inTemplate.data());
	}

}