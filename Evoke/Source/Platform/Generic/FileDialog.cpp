#include "PCH.h"
#include "FileDialog.h"
#include "Filesystem.h"

#include "nfd.h"

namespace Evoke
{
	std::optional<String> FileDialog::Save(StringView inDefaultPath, StringView inFilter)
	{
		nfdchar_t* rawPath{ nullptr };
		nfdresult_t result = NFD_SaveDialog(inFilter.data(), Filesystem::Absolute(inDefaultPath).data(), &rawPath);
		switch (result)
		{
		case NFD_OKAY:
		{
			String outPath{ rawPath };
			delete rawPath;
			return outPath;
		}
		case NFD_CANCEL:
			return std::nullopt;
		default:
		case NFD_ERROR:
			EV_LOG(LogEngine, ELogLevel::Error, "{}", NFD_GetError());
			return std::nullopt;
		}
	}

	std::optional<String> FileDialog::Open(StringView inDefaultPath, StringView inFilter)
	{
		nfdchar_t* rawPath{ nullptr };
		nfdresult_t result = NFD_OpenDialog(inFilter.data(), Filesystem::Absolute(inDefaultPath).data(), &rawPath);
		switch (result)
		{
		case NFD_OKAY:
		{
			String outPath{ rawPath };
			delete rawPath;
			return outPath;
		}
		case NFD_CANCEL:
			return std::nullopt;
		default:
		case NFD_ERROR:
			EV_LOG(LogEngine, ELogLevel::Error, "{}", NFD_GetError());
			return std::nullopt;
		}
	}

	std::optional<std::vector<String>> FileDialog::OpenMultiple(StringView inDefaultPath, StringView inFilter)
	{
		nfdpathset_t pathSet;
		nfdresult_t result = NFD_OpenDialogMultiple(inFilter.data(), Filesystem::Absolute(inDefaultPath).data(), &pathSet);
		switch (result)
		{
		case NFD_OKAY:
		{
			std::vector<String> paths;
			for (size_t i = 0; i < NFD_PathSet_GetCount(&pathSet); i++)
			{
				nfdchar_t* path = NFD_PathSet_GetPath(&pathSet, i);
				paths.emplace_back(path);
			}
			NFD_PathSet_Free(&pathSet);

			return paths;
		}
		case NFD_CANCEL:
			return std::nullopt;
		default:
		case NFD_ERROR:
			EV_LOG(LogEngine, ELogLevel::Error, "{}", NFD_GetError());
			return std::nullopt;
		}
	}

	std::optional<String> FileDialog::OpenFolder(StringView inDefaultPath)
	{
		nfdchar_t* rawPath{ nullptr };
		nfdresult_t result = NFD_PickFolder(Filesystem::Absolute(inDefaultPath).data(), &rawPath);
		switch (result)
		{
		case NFD_OKAY:
		{
			String outPath{ rawPath };
			delete rawPath;
			return outPath;
		}
		case NFD_CANCEL:
			return std::nullopt;
		default:
		case NFD_ERROR:
			EV_LOG(LogEngine, ELogLevel::Error, "{}", NFD_GetError());
			return std::nullopt;
		}
	}

}