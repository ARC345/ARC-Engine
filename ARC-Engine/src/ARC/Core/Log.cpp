#include "arc_pch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace arc
{
	std::shared_ptr<spdlog::logger> CLog::core_logger_;
	std::shared_ptr<spdlog::logger> CLog::client_logger_;

	int CLog::init()
	{
		spdlog::set_pattern("%^[%T]-%n[%l]: %v%$");
		core_logger_ = spdlog::stdout_color_mt("CORE");
		core_logger_->set_level(spdlog::level::trace);
		client_logger_ = spdlog::stdout_color_mt("APP");
		client_logger_->set_level(spdlog::level::trace);

		return 1;
	}
}