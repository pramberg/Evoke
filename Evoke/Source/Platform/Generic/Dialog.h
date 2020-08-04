#pragma once
#include "Core/Core.h"

#include <optional>

namespace Evoke
{
	class Dialog
	{
	public:
		enum class EButton : c8
		{
			Cancel,
			Ok,
			Yes,
			No,
			Abort,
			Retry,
			Ignore,
		};

		enum class EMessageType : c8
		{
			Ok = 0,
			OkCancel,
			YesNo,
			YesNoCancel,
			RetryCancel,
			AbortRetryIgnore,
		};

		enum class EIcon : c8
		{
			Info = 0,
			Warning,
			Error,
			Question,
		};

		/**
		 * Opens a save dialog window to save a single file.
		 *
		 * @param 	inTitle		 	(Optional) The title of the window.
		 * @param 	inInitialPath	(Optional) The path to start at.
		 * @param 	inFilter	 	(Optional) The file filter to use. Vector with alternating
		 * 								values, first the name of the filter, then the actual filter.
		 *
		 * @returns	The selected path, if the operation wasn't canceled.
		 */
		static std::optional<String> SaveFile(StringView inTitle = "Save file", StringView inInitialPath = ".", std::vector<String> inFilter = { "All Files", "*" });

		/**
		 * Opens a dialog window to open a single file.
		 *
		 * @param 	inTitle		 	(Optional) The title of the window.
		 * @param 	inInitialPath	(Optional) The path to start at.
		 * @param 	inFilter	 	(Optional) The file filter to use. Vector with alternating
		 * 							values, first the name of the filter, then the actual filter.
		 *
		 * @returns	The selected path, if the operation wasn't canceled.
		 */
		static std::optional<String> OpenFile(StringView inTitle = "Open file", StringView inInitialPath = ".", std::vector<String> inFilter = { "All Files", "*" });

		/**
		 * Opens a dialog window to open a multiple files.
		 *
		 * @param 	inTitle		 	(Optional) The title of the window.
		 * @param 	inInitialPath	(Optional) The path to start at.
		 * @param 	inFilter	 	(Optional) The file filter to use. Vector with alternating
		 * 							values, first the name of the filter, then the actual filter.
		 *
		 * @returns	The selected path, if the operation wasn't canceled.
		 */
		static std::optional<std::vector<String>> OpenMultipleFiles(StringView inTitle = "Open file(s)", StringView inInitialPath = ".", std::vector<String> inFilter = { "All Files", "*" });

		/**
		 * Opens a dialog window to select a folder.
		 *
		 * @param 	inTitle		 	(Optional) The title of the window.
		 * @param 	inInitialPath	(Optional) The path to start at.
		 *
		 * @returns	The selected path, if the operation wasn't canceled.
		 */
		static std::optional<String> SelectFolder(StringView inTitle = "Select folder", StringView inInitialPath = ".");

		/**
		 * Displays a message dialog window, with the specified message type.
		 *
		 * @param 	inTitle		 	The title of the window.
		 * @param 	inText		 	The main text to be displayed.
		 * @param 	inMessageType	(Optional) The button choices the dialog should have.
		 * @param 	inIcon		 	(Optional) The icon to show.
		 *
		 * @returns	The button that was pressed.
		 */
		static EButton Message(StringView inTitle, StringView inText, EMessageType inMessageType = EMessageType::OkCancel, EIcon inIcon = EIcon::Info);

		/**
		 * Displays an OS notification.
		 *
		 * @param 	inTitle	The title of the notification.
		 * @param 	inText 	The main text to be displayed.
		 * @param 	inIcon 	(Optional) The icon to show.
		 */
		static void Notification(StringView inTitle, StringView inText, EIcon inIcon = EIcon::Info);
	};
}