#include "PCH.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Evoke
{
	std::shared_ptr<spdlog::logger> Log::sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::sClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T](%s:%#) %n: %v%$");
		sCoreLogger = spdlog::stdout_color_mt("Evoke");
		sCoreLogger->set_level(spdlog::level::trace);

		sClientLogger = spdlog::stdout_color_mt("App");
		sClientLogger->set_level(spdlog::level::trace);
	}
}
