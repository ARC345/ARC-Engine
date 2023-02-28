#include "arc_pch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace ARC
{
	std::shared_ptr<spdlog::logger> CLog::mCoreLogger;
	std::shared_ptr<spdlog::logger> CLog::mClientLogger;

	int CLog::Init()
	{
		spdlog::set_pattern("%^[%T]-%n[%l]: %v%$");
		mCoreLogger = spdlog::stdout_color_mt("CORE");
		mCoreLogger->set_level(spdlog::level::trace);
		mClientLogger = spdlog::stdout_color_mt("APP");
		mClientLogger->set_level(spdlog::level::trace);

		return 1;
	}
}