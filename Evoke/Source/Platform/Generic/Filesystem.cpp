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

	String Filesystem::ExtractFilename(StringView inFilepath)
	{
		fs::path path(inFilepath);
		return path.filename().string();
	}

	String Filesystem::Absolute(StringView inFilepath)
	{
		return fs::absolute(inFilepath).string();
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
}