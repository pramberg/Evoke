#include "PCH.h"
#include "Filesystem.h"

namespace fs = std::filesystem;

namespace Evoke
{
	string Filesystem::ReadFile(const string& inFilepath)
	{
		std::ifstream filestream(inFilepath);
		std::stringstream buffer;
		buffer << filestream.rdbuf();
		return buffer.str();
	}

	string Filesystem::ExtractFilename(const string& inFilepath)
	{
		fs::path path(inFilepath);
		return path.filename().string();
	}

	string Filesystem::Absolute(const string& inFilepath)
	{
		return fs::absolute(inFilepath).string();
	}

	b8 Filesystem::MatchPattern(const c8* inString, const c8* inPattern)
	{
		if (*inString == '\0' && *inPattern == '\0')
			return true;

		if (*inPattern == '*' && *(inPattern + 1) != '\0' && *inString == '\0')
			return false;

		if (*inPattern == '?' || *inPattern == *inString)
			return MatchPattern(inString + 1, inPattern + 1);

		if (*inPattern == '*')
			return MatchPattern(inString, inPattern + 1) || MatchPattern(inString + 1, inPattern);
		return false;
	}

}