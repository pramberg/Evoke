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

	string Filesystem::GetFilename(const string& inFilepath)
	{
		fs::path path(inFilepath);
		return path.filename().string();
	}
}
