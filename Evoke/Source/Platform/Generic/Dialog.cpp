#include "PCH.h"
#include "Dialog.h"
#include "Filesystem.h"

#include "portable-file-dialogs.h"

namespace Evoke
{
	static void AddFilterToNames(std::vector<String>& inFilter)
	{
		EV_CORE_ASSERT(inFilter.size() % 2 == 0, "Invalid filter layout. Even index should have the filter name, uneven filter definition.");
		for (size_t i = 0; i < inFilter.size(); i += 2)
		{
			String& name = inFilter[i];
			const String& filter = inFilter[i + 1];
			name += " (" + filter + ")";
		}
	}

	std::optional<String> Dialog::SaveFile(StringView inTitle, StringView inInitialPath, std::vector<String> inFilter)
	{
		AddFilterToNames(inFilter);

		auto dialog = pfd::save_file(inTitle.data(), inInitialPath.data(), inFilter).result();
		if (dialog.empty())
			return std::nullopt;
		return dialog;
	}

	std::optional<String> Dialog::OpenFile(StringView inTitle, StringView inInitialPath, std::vector<String> inFilter)
	{
		AddFilterToNames(inFilter);

		auto dialog = pfd::open_file(inTitle.data(), inInitialPath.data(), inFilter).result();
		if (dialog.empty())
			return std::nullopt;
		return dialog[0];
	}

	std::optional<std::vector<String>> Dialog::OpenMultipleFiles(StringView inTitle, StringView inInitialPath, std::vector<String> inFilter)
	{
		AddFilterToNames(inFilter);

		auto dialog = pfd::open_file(inTitle.data(), inInitialPath.data(), inFilter, pfd::opt::multiselect).result();
		if (dialog.empty())
			return std::nullopt;
		return dialog;
	}

	std::optional<String> Dialog::SelectFolder(StringView inTitle, StringView inInitialPath)
	{
		auto dialog = pfd::select_folder(inTitle.data(), inInitialPath.data()).result();
		if (dialog.empty())
			return std::nullopt;
		return dialog;
	}

	Dialog::EButton Dialog::MessageBox(StringView inTitle, StringView inText, EMessageType inMessageType, EIcon inIcon)
	{
		return (EButton)pfd::message(inTitle.data(), inText.data(), (pfd::choice)inMessageType, (pfd::icon)inIcon).result();
	}

	void Dialog::Notification(StringView inTitle, StringView inText, EIcon inIcon)
	{
		[[maybe_unused]] auto notification = pfd::notify(inTitle.data(), inText.data(), (pfd::icon)inIcon);
	}

}