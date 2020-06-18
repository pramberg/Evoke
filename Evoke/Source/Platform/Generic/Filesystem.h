#pragma once
#include "Core/Core.h"

namespace Evoke
{
	class Filesystem
	{
	public:
		static string ReadFile(const string& inFilepath);
		static string GetFilename(const string& inFilepath);
	};
}