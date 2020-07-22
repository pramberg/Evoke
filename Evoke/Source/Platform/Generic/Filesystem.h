#pragma once
#include "Core/Core.h"

namespace Evoke
{
	class Filesystem
	{
	public:
		static string ReadFile(const string& inFilepath);
		static string ExtractFilename(const string& inFilepath);
		static string Absolute(const string& inFilepath);
		static constexpr c8 Separator = std::filesystem::path::preferred_separator;

		/** Matches a string with a pattern that can have wildcards. */
		static b8 MatchPattern(const c8* inString, const c8* inPattern);
	};
}