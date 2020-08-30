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
	};
}