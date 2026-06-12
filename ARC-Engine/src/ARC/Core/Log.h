#pragma once

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#pragma warning(pop)

#include "ARC/Helpers/helpers.h"
#include "Macros.h"

namespace arc
{
	class ARC_API CLog
	{
	public:
		static int init();
		//inline static void SetPattern(std::string& newPattern, pattern_time_type newTimeType = pattern_time_type::local ){ spdlog::set_pattern(newPattern, newTimeType);};
	public:
		static std::shared_ptr<spdlog::logger>& get_core_logger() { return core_logger_; };
		static std::shared_ptr<spdlog::logger>& get_client_logger() { return client_logger_; };
	private:
		static std::shared_ptr<spdlog::logger> core_logger_;
		static std::shared_ptr<spdlog::logger> client_logger_;
	};
}

#define ARC_CORE_TRACE(...) ::arc::CLog::get_core_logger()->trace(__VA_ARGS__)
#define ARC_CORE_INFO(...)  ::arc::CLog::get_core_logger()->info(__VA_ARGS__)
#define ARC_CORE_WARN(...)  ::arc::CLog::get_core_logger()->warn(__VA_ARGS__)
#define ARC_CORE_ERROR(...) ::arc::CLog::get_core_logger()->error(__VA_ARGS__)
#define ARC_CORE_CRITICAL(...) ::arc::CLog::get_core_logger()->critical(__VA_ARGS__)

#define ARC_TRACE(...) ::arc::CLog::get_client_logger()->trace(__VA_ARGS__)
#define ARC_INFO(...)  ::arc::CLog::get_client_logger()->info(__VA_ARGS__)
#define ARC_WARN(...)  ::arc::CLog::get_client_logger()->warn(__VA_ARGS__)
#define ARC_ERROR(...) ::arc::CLog::get_client_logger()->error(__VA_ARGS__)
#define ARC_CRITICAL(...) ::arc::CLog::get_client_logger()->critical(__VA_ARGS__)