#pragma once
#include "Core.h"

// Set the SPDLOG active level to trace. We handle that ourselves using macros and set_level()
#define SPDLOG_ACTIVE_LEVEL 0
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#pragma warning(pop)

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

// Make GLM types printable with fmt. I couldn't find a generic way to do this, but if there is a way I'd like to do that instead
// of this...
#include <spdlog/fmt/bundled/format.h>
#include <glm/gtx/string_cast.hpp>

#define GLM_FORMAT_TYPE(inGLMType) \
template <>\
struct fmt::formatter<inGLMType> : formatter<string_view>\
{\
	template <typename FormatContext> \
	auto format(inGLMType inType, FormatContext& inContext) {\
		return formatter<string_view>::format(glm::to_string(inType), inContext);\
	}\
};\

GLM_FORMAT_TYPE(glm::vec1);
GLM_FORMAT_TYPE(glm::vec2);
GLM_FORMAT_TYPE(glm::vec3);
GLM_FORMAT_TYPE(glm::vec4);
GLM_FORMAT_TYPE(glm::mat2);
GLM_FORMAT_TYPE(glm::mat3);
GLM_FORMAT_TYPE(glm::mat4);
GLM_FORMAT_TYPE(glm::quat);