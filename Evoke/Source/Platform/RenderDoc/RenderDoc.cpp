#include "PCH.h"
#include "RenderDoc.h"
#include "Platform/Generic/Filesystem.h"
#include "Core/CommandLine.h"

#include "RLL/RLL.hpp"

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

	// #TEMP
	String ConvertToPlatformName(StringView inName)
	{
		String prefix;
#ifndef EV_PLATFORM_WINDOWS
		prefix = "lib";
#endif
		return prefix + inName.data() + rll::shared_library::get_platform_suffix();
	}

	TUniquePtr<RENDERDOC_API_1_1_2> RenderDoc::sRenderDocAPI;
	u32 RenderDoc::sRenderDocProcessID = 0;
	rll::shared_library RenderDoc::sRenderDocLib;

	b8 RenderDoc::TryInject(StringView inPathToRenderDoc)
	{
		auto path = std::filesystem::path(inPathToRenderDoc);
		path.make_preferred();
		if (path.has_filename())
			path += Filesystem::Separator;
		path += ConvertToPlatformName("renderdoc");

		try { sRenderDocLib.load(path.string()); }
		catch (const rll::exception::library_loading_error&) { return false; }

		using GetAPISignature = i32(RENDERDOC_Version, void**);

		const auto renderDocGetAPIFn = sRenderDocLib.get_function_symbol<GetAPISignature>("RENDERDOC_GetAPI");
		const i32 result = renderDocGetAPIFn(eRENDERDOC_API_Version_1_1_2, (void**)&sRenderDocAPI);
		
		EV_CORE_ASSERT(result == 1, "Failed to initialize RenderDoc.");

		SetCaptureTemplate();
		if (!CommandLine::HasParameter("-RDUI")) // #TODO: This should not be set via command line, move to preferences.
			sRenderDocAPI->MaskOverlayBits(eRENDERDOC_Overlay_None, eRENDERDOC_Overlay_None);
		return true;
	}

	void RenderDoc::Close()
	{
		if (sRenderDocLib.is_loaded())
			sRenderDocLib.unload();
		sRenderDocAPI.release();
	}

	void RenderDoc::LaunchUI(StringView inCommandLine)
	{
		if (!IsInitialized())
			return;
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
		if (!IsInitialized())
			return;
		sRenderDocAPI->TriggerCapture();
	}

	void RenderDoc::SetCaptureTemplate(StringView inTemplate)
	{
		if (!IsInitialized())
			return;
		sRenderDocAPI->SetCaptureFilePathTemplate(inTemplate.data());
	}

	b8 RenderDoc::IsInitialized()
	{
		return (b8)sRenderDocAPI && sRenderDocLib.is_loaded();
	}
}