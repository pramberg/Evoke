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
	};

	enum class ELogLevel
	{
		Trace = ::spdlog::level::trace,
		Info = ::spdlog::level::info,
		Warning = ::spdlog::level::warn,
		Error = ::spdlog::level::err,
		Critical = ::spdlog::level::critical,
		Disabled = ::spdlog::level::off,
		Count
	};
}

// Logging macros
#define EV_TRACE ::Evoke::ELogLevel::Trace
#define EV_INFO ::Evoke::ELogLevel::Info
#define EV_WARNING ::Evoke::ELogLevel::Warning
#define EV_ERROR ::Evoke::ELogLevel::Error
#define EV_CRITICAL ::Evoke::ELogLevel::Critical

// Using SPDLOG macros will ensure all patterns work correctly, like the ones using __LINE__ and __FILE__.
#ifndef EV_RETAIL

/**
 * Gets a logger from its name. Don't use quotes, syntax is similar to UE4's logging.
 * Currently uses spdlog's implementation since they already have a registry setup
 *
 * @param 	inName	The name of the logger. Note: don't use quotes.
 */
#define EV_GET_LOGGER(inName) ::spdlog::details::registry::instance().get(#inName)

/**
 * Creates a new logger and makes it available using EV_GET_LOGGER().
 *
 * @param 	inLoggerName  	The name of the logger. Note: all loggers should be prefixed with "Log".
 * @param 	inDefaultLevel	The default log level.
 */
#define EV_ADD_LOGGER(inLoggerName, inDefaultLevel) { auto inLoggerName = ::spdlog::stdout_color_mt(#inLoggerName); inLoggerName->set_level((spdlog::level::level_enum)inDefaultLevel); }

/**
 * The main logging macro. Follows UE4 logging syntax.
 *
 * @param 	inLoggerName	Name of the logger.
 * @param 	inLevel			The log level. Use either EV_*LEVEL* or ELogLevel::*Level*.
 * @param 	...				First parameter determines the formatting, followed by any number of
 * 							values that should replace any {} or {N} in the format string.
 */
#define EV_LOG(inLoggerName, inLevel, ...) SPDLOG_LOGGER_CALL(EV_GET_LOGGER(inLoggerName), (spdlog::level::level_enum)inLevel, __VA_ARGS__)

/**
 * Similar to EV_LOG, but allows for specifying a custom file and line number. This can be used
 * to specify errors in a shader for example.
 *
 * @param 	inLoggerName	Name of the logger.
 * @param 	inLevel			The log level. Use either EV_*LEVEL* or ELogLevel::*Level*.
 * @param 	inFile			The name or path to a file.
 * @param 	inLine			The line number.
 * @param 	...				First parameter determines the formatting, followed by any number of
 * 							values that should replace any {} or {N} in the format string.
 */
#define EV_LOG_CUSTOM_LOCATION(inLoggerName, inLevel, inFile, inLine, ...) EV_GET_LOGGER(inLoggerName)->log(\
	spdlog::source_loc{inFile, inLine, SPDLOG_FUNCTION},\
	(spdlog::level::level_enum)inLevel, __VA_ARGS__\
)

#else
#define EV_ADD_LOGGER(inLoggerName, inDefaultLevel)
#define EV_LOG(inLoggerName, inLevel, ...)
#define EV_LOG_CUSTOM_LOCATION(inLoggerName, inLevel, inFile, inLine, ...)
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