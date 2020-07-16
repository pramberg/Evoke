#include "PCH.h"
#include "Filewatcher.h"
#include "Filesystem.h"

#include <iterator>

namespace Evoke
{
	Filewatcher::Filewatcher() : mEfswFilewatcher(MakeUnique<efsw::FileWatcher>()), mListener(MakeUnique<FilewatcherListener>(this))
	{
		mEfswFilewatcher->watch();
	}

	Filewatcher::Filewatcher(const string& inPath, b8 inRecursive) : Filewatcher()
	{
		AddPath(inPath, true);
	}


	Filewatcher::Filewatcher(const std::initializer_list<std::pair<string, b8>> inList) : Filewatcher()
	{
		for (auto [path, recursive] : inList)
		{
			AddPath(path, recursive);
		}
	}

	Filewatcher::~Filewatcher()
	{
	}

	b8 Filewatcher::AddPath(const string& inPath, b8 inRecursive)
	{
		auto absPath = std::filesystem::absolute(inPath);
		const string absPathStr = absPath.string();
		if (std::filesystem::is_directory(absPath))
		{
			EV_ASSERT(absPathStr.find('*') == std::string::npos, "Asterisks in directory paths are not supported.");
			mEfswFilewatcher->addWatch(absPathStr, mListener.get(), inRecursive);
		}
		else
		{
			const size_t firstAsterisk = absPathStr.find('*');
			const size_t lastSubdir = absPathStr.find_last_of(std::filesystem::path::preferred_separator, firstAsterisk);
			const string directory = absPathStr.substr(0, lastSubdir + 1);
			string pattern = absPathStr.substr(lastSubdir + 1);
			if (pattern[0] != '*')
				pattern = '*' + pattern;

			mPatterns.push_back(pattern);
			mEfswFilewatcher->addWatch(directory, mListener.get(), true);
		}
		return true;
	}

	b8 Filewatcher::RemovePath(const string& inPath)
	{
		mEfswFilewatcher->removeWatch(inPath);
		return true;
	}

	b8 Filewatcher::MatchesPattern(const string& inFilename) const
	{
		for (const string& pattern : mPatterns)
		{
			if (Filesystem::MatchPattern(inFilename.c_str(), pattern.c_str()))
				return true;
		}
		return false;
	}

}