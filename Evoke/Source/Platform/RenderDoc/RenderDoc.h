#include "renderdoc_app.h"
#include "Core/Core.h"

namespace rll { class shared_library; }

namespace Evoke
{
	class RenderDoc
	{
	public:
		static b8 TryInject(StringView inPathToRenderDoc);
		static void Close();
		static RENDERDOC_API_1_1_2* API() { return sRenderDocAPI.get(); }
		static void LaunchUI(StringView inCommandLine = "");
		static void TriggerCapture();
		static void SetCaptureTemplate(StringView inTemplate = "RenderDoc/Captures/Evoke");
		static b8 IsInitialized();

	private:
		static TUniquePtr<RENDERDOC_API_1_1_2> sRenderDocAPI;
		static u32 sRenderDocProcessID;
		static rll::shared_library sRenderDocLib;
	};
}