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

#define EV_CORE_TRACE(...) SPDLOG_LOGGER_CALL(::Evoke::Log::GetCoreLogger(), spdlog::level::trace, __VA_ARGS__);
#define EV_CORE_INFO(...) SPDLOG_LOGGER_CALL(::Evoke::Log::GetCoreLogger(), spdlog::level::info, __VA_ARGS__);
#define EV_CORE_WARN(...) SPDLOG_LOGGER_CALL(::Evoke::Log::GetCoreLogger(), spdlog::level::warn, __VA_ARGS__);
#define EV_CORE_ERROR(...) SPDLOG_LOGGER_CALL(::Evoke::Log::GetCoreLogger(), spdlog::level::err, __VA_ARGS__);
#define EV_CORE_CRITICAL(...) SPDLOG_LOGGER_CALL(::Evoke::Log::GetCoreLogger(), spdlog::level::critical, __VA_ARGS__);

#define EV_TRACE(...) SPDLOG_LOGGER_CALL(::Evoke::Log::GetClientLogger(), spdlog::level::trace, __VA_ARGS__);
#define EV_INFO(...) SPDLOG_LOGGER_CALL(::Evoke::Log::GetClientLogger(), spdlog::level::info, __VA_ARGS__);
#define EV_WARN(...) SPDLOG_LOGGER_CALL(::Evoke::Log::GetClientLogger(), spdlog::level::warn, __VA_ARGS__);
#define EV_ERROR(...) SPDLOG_LOGGER_CALL(::Evoke::Log::GetClientLogger(), spdlog::level::err, __VA_ARGS__);
#define EV_CRITICAL(...) SPDLOG_LOGGER_CALL(::Evoke::Log::GetClientLogger(), spdlog::level::critical, __VA_ARGS__);