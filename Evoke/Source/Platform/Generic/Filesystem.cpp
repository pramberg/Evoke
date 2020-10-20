#include "PCH.h"
#include "Filesystem.h"

namespace fs = std::filesystem;

namespace Evoke
{
	String Filesystem::ReadFile(StringView inFilepath)
	{
		std::ifstream filestream(inFilepath);
		std::stringstream buffer;
		buffer << filestream.rdbuf();
		return buffer.str();
	}

	String Filesystem::Filename(StringView inFilepath)
	{
		fs::path path(inFilepath);
		return path.filename().replace_extension().string();
	}

	String Filesystem::Extension(StringView inFilepath)
	{
		fs::path path(inFilepath);
		return path.extension().string();
	}

	String Filesystem::Path(StringView inFilepath)
	{
		fs::path path(inFilepath);
		return path.has_filename() ? path.parent_path().string() : path.string();
	}

	String Filesystem::Absolute(StringView inFilepath)
	{
		return fs::absolute(inFilepath).string();
	}

	String Filesystem::Relative(StringView inFilepath, StringView inBasePath)
	{
		return fs::relative(inFilepath, inBasePath).string();
	}

	fs::directory_iterator Filesystem::IterateDirectory(StringView inFilepath)
	{
		return fs::directory_iterator(inFilepath);
	}

	std::filesystem::recursive_directory_iterator Filesystem::IterateDirectoryRecursive(StringView inFilepath)
	{
		return fs::recursive_directory_iterator(inFilepath);
	}

	b8 Filesystem::MatchPattern(StringView inString, StringView inPattern)
	{
		return MatchPatternImpl(inString.data(), inPattern.data());
	}

	b8 Filesystem::MatchPatternImpl(const c8* inString, const c8* inPattern)
	{
		if (*inString == '\0' && *inPattern == '\0')
			return true;

		if (*inPattern == '*' && *(inPattern + 1) != '\0' && *inString == '\0')
			return false;

		if (*inPattern == '?' || *inPattern == *inString)
			return MatchPatternImpl(inString + 1, inPattern + 1);

		if (*inPattern == '*')
			return MatchPatternImpl(inString, inPattern + 1) || MatchPatternImpl(inString + 1, inPattern);
		return false;
	}

	b8 Filesystem::Exists(StringView inPath)
	{
		return fs::exists(inPath);
	}

}