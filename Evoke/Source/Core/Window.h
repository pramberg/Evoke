#pragma once
#include "Core.h"
#include "Delegate.h"

namespace Evoke
{
	struct WindowProperties
	{
		string Title;
		u32 Width;
		u32 Height;

		WindowProperties(const string& inTitle = "Evoke Engine", u32 inWidth = 1280, u32 inHeight = 720)
			: Title(inTitle), Width(inWidth), Height(inHeight) {}
	};

	enum class ECursorMode
	{
		Normal = 0x00034001,
		Hidden = 0x00034002,
		Disabled = 0x00034003
	};

	class Window
	{
	public:
		virtual ~Window() {}
	
		virtual void Update(f32 inDeltaTime) = 0;
	
		virtual u32 Width() const = 0;
		virtual u32 Height() const = 0;

		virtual void SetVSync(b8 inEnabled) = 0;
		virtual b8 VSyncEnabled() const = 0;

		virtual void SetCursorMode(ECursorMode inCursorMode = ECursorMode::Normal) = 0;

		inline virtual void* NativeWindow() const = 0;

		static TUniquePtr<Window> Create(const WindowProperties& inProperties = WindowProperties());

	public:
		/**
		 * Event that gets broadcast when the window is resized.
		 *
		 * @param 	inWidth 	New window width.
		 * @param 	inHeight	New window height.
		 */
		MulticastDelegate<u32, u32> OnWindowResized;

		/** Event that gets broadcast when the window is closed. */
		MulticastDelegate<> OnWindowClosed;

		/**
		 * Event that gets broadcast when a key is pressed.
		 *
		 * @param 	inKeyCode	 	The key that was pressed.
		 * @param 	inRepeatCount	0 if it was the first press, 1 if it is a repeating key press.
		 */
		MulticastDelegate<EKeyCode, i32> OnKeyPressed;

		/**
		 * Event that gets broadcast when a key is released.
		 *
		 * @param 	inKeyCode	The key that was released.
		 */
		MulticastDelegate<EKeyCode> OnKeyReleased;

		/**
		 * Event that gets broadcast when the mouse is moved.
		 *
		 * @param 	inPositionX	The new x position relative to the window.
		 * @param 	inPositionY	The new y position relative to the window.
		 */
		MulticastDelegate<f32, f32> OnMouseMoved;

		/**
		 * Event that gets broadcast when a mouse button is pressed.
		 *
		 * @param 	inButton   	The button that was pressed.
		 * @param 	inNumClicks	1 if it was a single click, 2 if it was double.
		 */
		MulticastDelegate<EMouseButton, i32> OnMouseButtonPressed;

		/**
		 * Event that gets broadcast when a mouse button is released.
		 *
		 * @param 	inButton	 	The button that was released.
		 */
		MulticastDelegate<EMouseButton> OnMouseButtonReleased;

		/**
		 * Event that gets broadcast when a mouse is scrolled.
		 *
		 * @param 	inScrollX	How much was scrolled horizontally.
		 * @param 	inScrollY	How much was scrolled vertically.
		 */
		MulticastDelegate<f32, f32> OnMouseScrolled;
	};
}

namespace magic_enum::customize
{
	template <>
	struct enum_range<Evoke::ECursorMode>
	{
		static constexpr i32 min = 212993;
		static constexpr i32 max = 212995;
	};
}