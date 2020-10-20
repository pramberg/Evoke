#pragma once
#include "Core/Core.h"
#include "Core/Delegate.h"

namespace Evoke
{
	class Widget
	{
	public:
		Widget(StringView inTitle);
		virtual ~Widget() = default;

		virtual void Render() {}
		virtual void PreRender() {}
		virtual void PostRender() {}
		virtual void Resized() {} // #Q: Should this be a delegate?
		b8 WasClosed() { return !mIsOpen; }
		b8 IsHovered() { return mIsHovered; }
		b8 IsFocused() { return mIsFocused; }

		using WidgetFactoryFn = std::function<TUniquePtr<Widget>()>;
		static b8 AddWidgetFactory(const String& inName, WidgetFactoryFn inWidgetFactory);

		static std::map<String, WidgetFactoryFn>& Widgets()
		{
			return sWidgets;
		}
	protected:
		String mTitle;
		u32 mWidth = 0, mHeight = 0;
		b8 mIsHovered = false, mIsFocused = false;

	private:
		friend class ImGuiLayer;

		void RenderInternal();
		void StartFrame();
		void EndFrame();
		void ResizedInternal(u32 inWidth, u32 inHeight);

		b8 mIsOpen = true;

		// #TODO: This should be a more sophisticated system
		static u64 sWidgetID;
		static std::map<String, WidgetFactoryFn> sWidgets;
	};

	class WidgetManager
	{
	public:
		using WidgetFactoryFn = std::function<TUniquePtr<Widget>()>;

		void AddWidgetFactory(const String& inName, const WidgetFactoryFn& inWidgetFactory);
		void Each(const std::function<void(StringView, const WidgetFactoryFn&, i32)>& inFn);

		static WidgetManager& Get();

	private:
		std::map<String, WidgetFactoryFn> mWidgets;
	};

#define REGISTER_WIDGET(inWidget) WidgetManager::Get().AddWidgetFactory(#inWidget, [](){ return MakeUnique<inWidget>(); })
#define REGISTER_WIDGET_NAME(inWidget, inName) WidgetManager::Get().AddWidgetFactory(inName, [](){ return MakeUnique<inWidget>(); })
}