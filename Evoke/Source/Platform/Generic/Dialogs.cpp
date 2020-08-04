#include "PCH.h"
#include "Dialogs.h"
#include "Filesystem.h"

#include "portable-file-dialogs.h"

namespace Evoke
{
	std::optional<String> Dialogs::SaveFile(StringView inTitle, StringView inInitialPath, std::vector<String> inFilter)
	{
		auto dialog = pfd::save_file(inTitle.data(), inInitialPath.data(), inFilter).result();
		if (dialog.empty())
			return std::nullopt;
		return dialog;
	}

	std::optional<String> Dialogs::OpenFile(StringView inTitle, StringView inInitialPath, std::vector<String> inFilter)
	{
		auto dialog = pfd::open_file(inTitle.data(), inInitialPath.data(), inFilter).result();
		if (dialog.empty())
			return std::nullopt;
		return dialog[0];
	}

	std::optional<std::vector<String>> Dialogs::OpenMultipleFiles(StringView inTitle, StringView inInitialPath, std::vector<String> inFilter)
	{
		auto dialog = pfd::open_file(inTitle.data(), inInitialPath.data(), inFilter, pfd::opt::multiselect).result();
		if (dialog.empty())
			return std::nullopt;
		return dialog;
	}

	std::optional<String> Dialogs::SelectFolder(StringView inTitle, StringView inInitialPath)
	{
		auto dialog = pfd::select_folder(inTitle.data(), inInitialPath.data()).result();
		if (dialog.empty())
			return std::nullopt;
		return dialog;
	}

}