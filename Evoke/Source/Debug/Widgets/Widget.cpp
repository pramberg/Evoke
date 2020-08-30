#include "PCH.h"
#include "Widget.h"

#include <imgui.h>

namespace Evoke
{
	u64 Widget::sWidgetID = 0;
	Widget::Widget(StringView inTitle) : mTitle(inTitle)
	{
		mTitle = mTitle + "##" + std::to_string(sWidgetID++);
	}

	void Widget::StartFrame()
	{
		PreRender();
		if (!ImGui::Begin(mTitle.c_str(), &mIsOpen))
			return;

		mIsFocused = ImGui::IsWindowFocused();
		mIsHovered = ImGui::IsWindowHovered();

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		if (panelSize.x != mWidth || panelSize.y != mHeight)
			ResizedInternal((u32)panelSize.x, (u32)panelSize.y);
	}

	void Widget::EndFrame()
	{
		ImGui::End();
		PostRender();
	}

	void Widget::ResizedInternal(u32 inWidth, u32 inHeight)
	{
		mWidth = inWidth;
		mHeight = inHeight;
		Resized();
	}

	void Widget::RenderInternal()
	{
		if (!mIsOpen)
			return;

		StartFrame();
		if (!mIsOpen)
		{
			EndFrame();
			return;
		}

		Render();
		EndFrame();
	}

}