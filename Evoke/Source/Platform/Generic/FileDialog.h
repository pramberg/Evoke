#pragma once
#include "Core/Core.h"

#include <optional>

namespace Evoke
{
	// #TODO: Think about having a version that isn't static, that keeps track of previous default path.
	class FileDialog
	{
	public:
		/**
		 * Opens a save dialog window to save a single file.
		 * @param 	inDefaultPath	The path to start at.
		 * @param 	inFilter	 	The file filter to use. Use ',' for multiple types, ';' for a new filter.
		 * @returns	The selected path, if the operation wasn't canceled.
		 */
		static std::optional<String> Save(StringView inDefaultPath, StringView inFilter);

		/**
		 * Opens a dialog window to open a single file.
		 * @param 	inDefaultPath	The path to start at.
		 * @param 	inFilter	 	The file filter to use. Use ',' for multiple types, ';' for a new filter.
		 * @returns	The selected path, if the operation wasn't canceled.
		 */
		static std::optional<String> Open(StringView inDefaultPath, StringView inFilter);

		/**
		 * Opens a dialog window to open a multiple files.
		 * @param 	inDefaultPath	The path to start at.
		 * @param 	inFilter	 	The file filter to use. Use ',' for multiple types, ';' for a new filter.
		 * @returns	The selected path, if the operation wasn't canceled.
		 */
		static std::optional<std::vector<String>> OpenMultiple(StringView inDefaultPath, StringView inFilter);

		/**
		 * Opens a dialog window to open a folder.
		 * @param 	inDefaultPath	The path to start at.
		 * @param 	inFilter	 	The file filter to use. Use ',' for multiple types, ';' for a new filter.
		 * @returns	The selected path, if the operation wasn't canceled.
		 */
		static std::optional<String> OpenFolder(StringView inDefaultPath);
	};
}