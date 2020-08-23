#pragma once
#include "Core/Core.h"

#include <optional>

namespace Evoke
{
	/** Provides static access to command line arguments. '-' is the default character used to determine parameter names. */
	class CommandLine
	{
	public:
		/**
		 * Initializes this static class. Should not be called except for the first thing in main().
		 * @param 	inArgCount 	Number of arguments.
		 * @param   inArgValues	The argument values.
		 */
		static void Init(i32 inArgCount, c8* inArgValues[]);

		/**
		 * Was a certain parameter specified?
		 * @param 	inParameter	The parameter.
		 * @returns True if the parameter was specified, false if it wasn't.
		 */
		static b8 HasParameter(StringView inParameter);

		/**
		 * Gets the arguments after a parameter, until a new parameter is specified, or until the end of the argument list.
		 * @param 	inParameter	The parameter, starting with a '-'.
		 * @returns	The arguments.
		 */
		static std::optional<std::vector<String>> Arguments(StringView inParameter);

		/**
		 * Gets the argument after a parameter, at a specified index.
		 * @param 	inParameter	The parameter, starting with a '-'.
		 * @param 	inIndex	   	(Optional) What argument to get from the vector. 0 will return the first argument.
		 * @returns	The argument.
		 */
		static std::optional<String> Argument(StringView inParameter, u64 inIndex = 0);

	private:
		static std::unordered_map<String, std::vector<String>> mParameterMap;
	};
}