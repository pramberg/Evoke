#include "PCH.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Evoke
{
	void Log::Init()
	{
		spdlog::set_pattern("%^[%T](%s:%#) %n: %v%$");

		EV_ADD_LOGGER(LogEngine, ELogLevel::Trace);
		EV_ADD_LOGGER(LogRHI, ELogLevel::Trace);
		EV_ADD_LOGGER(LogApp, ELogLevel::Trace);
		EV_ADD_LOGGER(LogTemp, ELogLevel::Trace);
		EV_ADD_LOGGER(LogShader, ELogLevel::Trace);
	}
}
