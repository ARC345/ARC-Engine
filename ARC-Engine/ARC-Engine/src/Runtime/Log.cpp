#include "Log.h"
#include "Helpers/macros.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace ARC
{
	std::shared_ptr<spdlog::logger> CLog::m_CoreLogger;
	std::shared_ptr<spdlog::logger> CLog::m_ClientLogger;

	int CLog::Init()
	{
		spdlog::set_pattern("%^[%T]-%n[%l]: %v%$");
		m_CoreLogger = spdlog::stdout_color_mt("CORE");
		m_CoreLogger->set_level(spdlog::level::trace);
		m_ClientLogger = spdlog::stdout_color_mt("APP");
		m_ClientLogger->set_level(spdlog::level::trace);

		return 1;
	}
}