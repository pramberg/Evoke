#pragma once
#include "Core/Core.h"

namespace ImGui
{
	b8 Combo(const c8* inLabel, i32* inCurrentItem, const std::vector<std::string>& inItems, i32 inHeightInItems = -1);
}