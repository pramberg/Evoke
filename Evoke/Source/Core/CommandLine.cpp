#include "PCH.h"
#include "CommandLine.h"

namespace Evoke
{
	std::unordered_map<String, std::vector<String>> CommandLine::mParameterMap;

	void CommandLine::Init(i32 inArgCount, c8* inArgValues[])
	{
		EV_CORE_ASSERT(mParameterMap.empty(), "CommandLine::Init() should only be run once.");

		StringView currentParameter;
		// Start at 1 because the first value is path to executable.
		for (i32 i = 1; i < inArgCount; i++)
		{
			if (inArgValues[i][0] == '-')
			{
				currentParameter = inArgValues[i];
				mParameterMap[currentParameter.data()] = std::vector<String>();
			}
			else if (HasParameter(currentParameter))
			{
				mParameterMap[currentParameter.data()].emplace_back(inArgValues[i]);
			}
		}
	}

	b8 CommandLine::HasParameter(StringView inParameter)
	{
		return mParameterMap.find(inParameter.data()) != mParameterMap.end();
	}

	std::optional<std::vector<String>> CommandLine::Arguments(StringView inParameter)
	{
		if (!HasParameter(inParameter))
			return std::nullopt;
		return mParameterMap[inParameter.data()];
	}
}