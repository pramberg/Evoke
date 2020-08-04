#include "PCH.h"
#include "FileDialog.h"
#include "Filesystem.h"

#include "nfd.h"

namespace Evoke
{
	b8 IsNFDDialogSuccessful(nfdresult_t inResult)
	{
		switch (inResult)
		{
		case NFD_OKAY:
			return true;
		case NFD_CANCEL:
			return false;
		default:
		case NFD_ERROR:
			EV_LOG(LogEngine, ELogLevel::Error, "{}", NFD_GetError());
			return false;
		}
	}

	std::optional<String> FileDialog::Save(std::optional<StringView> inDefaultPath, std::optional<StringView> inFilter)
	{
		nfdchar_t* rawPath{ nullptr };
		nfdresult_t result = NFD_SaveDialog(
			inFilter ? inFilter->data() : nullptr,
			inDefaultPath ? Filesystem::Absolute(*inDefaultPath).data() : nullptr,
			&rawPath
		);
		if (!IsNFDDialogSuccessful(result))
			return std::nullopt;

		String outPath{ rawPath };
		delete rawPath;
		return outPath;
	}

	std::optional<String> FileDialog::Open(std::optional<StringView> inDefaultPath, std::optional<StringView> inFilter)
	{
		nfdchar_t* rawPath{ nullptr };
		nfdresult_t result = NFD_OpenDialog(
			inFilter ? inFilter->data() : nullptr,
			inDefaultPath ? Filesystem::Absolute(*inDefaultPath).data() : nullptr,
			&rawPath
		);
		if (!IsNFDDialogSuccessful(result))
			return std::nullopt;

		String outPath{ rawPath };
		delete rawPath;
		return outPath;
	}

	std::optional<std::vector<String>> FileDialog::OpenMultiple(std::optional<StringView> inDefaultPath, std::optional<StringView> inFilter)
	{
		nfdpathset_t pathSet;
		nfdresult_t result = NFD_OpenDialogMultiple(
			inFilter ? inFilter->data() : nullptr,
			inDefaultPath ? Filesystem::Absolute(*inDefaultPath).data() : nullptr,
			&pathSet
		);

		if (!IsNFDDialogSuccessful(result))
			return std::nullopt;

		std::vector<String> paths;
		for (size_t i = 0; i < NFD_PathSet_GetCount(&pathSet); i++)
			paths.emplace_back(NFD_PathSet_GetPath(&pathSet, i));
		NFD_PathSet_Free(&pathSet);
		return paths;
	}

	std::optional<String> FileDialog::OpenFolder(std::optional<StringView> inDefaultPath)
	{
		nfdchar_t* rawPath{ nullptr };
		nfdresult_t result = NFD_PickFolder(
			inDefaultPath ? Filesystem::Absolute(*inDefaultPath).data() : nullptr,
			&rawPath
		);
		if (!IsNFDDialogSuccessful(result))
			return std::nullopt;

		String outPath{ rawPath };
		delete rawPath;
		return outPath;
	}

}