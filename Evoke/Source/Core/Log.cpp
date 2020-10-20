#include "PCH.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Debug/Widgets/ConsoleWidget.h"

namespace Evoke
{
	void Log::Init()
	{

		/*{
			auto inLoggerName = MakeShared<::spdlog::logger>("inLoggerName",
		{
				MakeShared<::spdlog::sinks::stdout_color_sink_mt>(),
				MakeShared<::spdlog::sinks::msvc_sink_mt>()
		});
			inLoggerName->set_level((::spdlog::level::level_enum)inDefaultLevel);
			spdlog::register_logger(inLoggerName); \
		}*/

		EV_ADD_LOGGER(LogEngine, ELogLevel::Trace);
		EV_ADD_LOGGER(LogRHI, ELogLevel::Trace);
		EV_ADD_LOGGER(LogApp, ELogLevel::Trace);
		EV_ADD_LOGGER(LogTemp, ELogLevel::Trace);
		EV_ADD_LOGGER(LogShader, ELogLevel::Trace);
	}

	void ConsoleSink::sink_it_(const ::spdlog::details::log_msg& msg)
	{
		// If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
		::spdlog::memory_buf_t formatted;
		base_sink<std::mutex>::formatter_->format(msg, formatted);
		ConsoleWidget::GetMessages().emplace_back(fmt::to_string(formatted), (ELogLevel)msg.level);
	}

}
