#pragma once
#include "Core/Core.h"

namespace Evoke
{
	class Filesystem
	{
	public:
		static String ReadFile(StringView inFilepath);
		static String ExtractFilename(StringView inFilepath);
		static String Absolute(StringView inFilepath);

		/** Matches a string with a pattern that can have wildcards. */
		static b8 MatchPattern(StringView inString, StringView inPattern);

	public:
		static constexpr c8 Separator = std::filesystem::path::preferred_separator;

	private:
		/** #NOTE: Uses C-style strings because the function is recursive and it's noticeably faster: https://quick-bench.com/q/g4tH2Ox1y4KtNogS3LqUuzfI4uE */
		static b8 MatchPatternImpl(const c8* inString, const c8* inPattern);
	};
}