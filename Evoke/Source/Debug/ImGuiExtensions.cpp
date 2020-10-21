#include "PCH.h"
#include "ImGuiExtensions.h"

#include "imgui.h"

namespace ImGui
{
	b8 Combo(const c8* inLabel, i32* inCurrentItem, const std::vector<std::string>& inItems, i32 inHeightInItems)
	{
		return Combo(inLabel, inCurrentItem, [](void* inData, int inIndex, const char** outText) { *outText = (*((const std::vector<std::string>*)inData))[inIndex].c_str(); return true; }, (void*)&inItems, inItems.size(), inHeightInItems);
	}
}