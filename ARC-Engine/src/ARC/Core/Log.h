#pragma once

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#pragma warning(pop)

#include "ARC/Helpers/helpers.h"
#include "Macros.h"

namespace ARC
{
	class ARC_API CLog
	{
	public:
		static int Init();
		//inline static void SetPattern(std::string& newPattern, pattern_time_type newTimeType = pattern_time_type::local ){ spdlog::set_pattern(newPattern, newTimeType);};
	public:
		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return mCoreLogger; };
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return mClientLogger; };
	private:
		static std::shared_ptr<spdlog::logger> mCoreLogger;
		static std::shared_ptr<spdlog::logger> mClientLogger;
	};
}

#define ARC_CORE_TRACE(...) ::ARC::CLog::GetCoreLogger()->trace(__VA_ARGS__)
#define ARC_CORE_INFO(...)  ::ARC::CLog::GetCoreLogger()->info(__VA_ARGS__)
#define ARC_CORE_WARN(...)  ::ARC::CLog::GetCoreLogger()->warn(__VA_ARGS__)
#define ARC_CORE_ERROR(...) ::ARC::CLog::GetCoreLogger()->error(__VA_ARGS__)
#define ARC_CORE_CRITICAL(...) ::ARC::CLog::GetCoreLogger()->critical(__VA_ARGS__)

#define ARC_TRACE(...) ::ARC::CLog::GetClientLogger()->trace(__VA_ARGS__)
#define ARC_INFO(...)  ::ARC::CLog::GetClientLogger()->info(__VA_ARGS__)
#define ARC_WARN(...)  ::ARC::CLog::GetClientLogger()->warn(__VA_ARGS__)
#define ARC_ERROR(...) ::ARC::CLog::GetClientLogger()->error(__VA_ARGS__)
#define ARC_CRITICAL(...) ::ARC::CLog::GetClientLogger()->critical(__VA_ARGS__)