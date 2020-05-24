#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Evoke
{
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return sClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> sCoreLogger;
		static std::shared_ptr<spdlog::logger> sClientLogger;
	};
}

// Logging macros
// Using SPDLOG macros will ensure all patterns work correctly, like the ones using __LINE__ and __FILE__.
#ifndef EV_RETAIL
#define EV_CORE_TRACE(...) SPDLOG_LOGGER_TRACE(::Evoke::Log::GetCoreLogger(), __VA_ARGS__)
#define EV_CORE_INFO(...) SPDLOG_LOGGER_INFO(::Evoke::Log::GetCoreLogger(), __VA_ARGS__)
#define EV_CORE_WARN(...) SPDLOG_LOGGER_WARN(::Evoke::Log::GetCoreLogger(), __VA_ARGS__)
#define EV_CORE_ERROR(...) SPDLOG_LOGGER_ERROR(::Evoke::Log::GetCoreLogger(), __VA_ARGS__)
#define EV_CORE_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::Evoke::Log::GetCoreLogger(), __VA_ARGS__)

#define EV_TRACE(...) SPDLOG_LOGGER_TRACE(::Evoke::Log::GetClientLogger(), __VA_ARGS__)
#define EV_INFO(...) SPDLOG_LOGGER_INFO(::Evoke::Log::GetClientLogger(), __VA_ARGS__)
#define EV_WARN(...) SPDLOG_LOGGER_WARN(::Evoke::Log::GetClientLogger(), __VA_ARGS__)
#define EV_ERROR(...) SPDLOG_LOGGER_ERROR(::Evoke::Log::GetClientLogger(), __VA_ARGS__)
#define EV_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::Evoke::Log::GetClientLogger(), __VA_ARGS__)
#else
#define EV_CORE_TRACE(...)
#define EV_CORE_INFO(...)
#define EV_CORE_WARN(...)
#define EV_CORE_ERROR(...)
#define EV_CORE_CRITICAL(...)

#define EV_TRACE(...)
#define EV_INFO(...)
#define EV_WARN(...)
#define EV_ERROR(...)
#define EV_CRITICAL(...)
#endif