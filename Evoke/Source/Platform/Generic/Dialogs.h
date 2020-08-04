#pragma once
#include "Core/Core.h"

#include <optional>

namespace Evoke
{
	class Dialogs
	{
	public:
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
	};
}