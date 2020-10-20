#include "PCH.h"
#include "Widget.h"

#include <imgui.h>

namespace Evoke
{
	u64 Widget::sWidgetID = 0;
	std::map<String, Widget::WidgetFactoryFn> Widget::sWidgets = {};

	Widget::Widget(StringView inTitle) : mTitle(inTitle)
	{
		mTitle = mTitle + "##" + std::to_string(sWidgetID++);
	}

	b8 Widget::AddWidgetFactory(const String& inName, WidgetFactoryFn inWidgetFactory)
	{
		static auto& widgets = Widgets();
		widgets[inName] = inWidgetFactory;
		return true;
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

	void WidgetManager::AddWidgetFactory(const String& inName, const WidgetFactoryFn& inWidgetFactory)
	{
		mWidgets[inName] = inWidgetFactory;
	}

	void WidgetManager::Each(const std::function<void(StringView, const WidgetFactoryFn&, i32)>& inFn)
	{
		i32 index = 0;
		for (auto [name, factory] : mWidgets)
		{
			inFn(name, factory, index++);
		}
	}

	WidgetManager& WidgetManager::Get()
	{
		static WidgetManager widgetManager;
		return widgetManager;
	}

}