#pragma once
#include "Core/Core.h"
#include "Filesystem.h"

namespace Evoke
{
	class Paths
	{
	public:
		static String ProjectPath() { return gProjectPath; }

	private:
		static void SetProjectPath(StringView inPath) { EV_ASSERT(Filesystem::Exists(inPath), "Path does not exist."); gProjectPath = inPath; }

		static String gProjectPath;

		friend class Application;
	};

}