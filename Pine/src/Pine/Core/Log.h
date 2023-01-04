#pragma once

#include "Pine/Core/Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Pine
{
	class PINE_API Log
	{
	public:

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
		{
			return s_CoreLogger;
		}

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return s_ClientLogger;
		}

	private:

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

// Core log macros
#define PN_CORE_CRITICAL(...)	::Pine::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define PN_CORE_ERROR(...)		::Pine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PN_CORE_WARN(...)		::Pine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PN_CORE_INFO(...)		::Pine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PN_CORE_TRACE(...)		::Pine::Log::GetCoreLogger()->trace(__VA_ARGS__)

// Client log macros
#define PN_CRITICAL(...)		::Pine::Log::GetClientLogger()->critical(__VA_ARGS__)
#define PN_ERROR(...)			::Pine::Log::GetClientLogger()->error(__VA_ARGS__)
#define PN_WARN(...)			::Pine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PN_INFO(...)			::Pine::Log::GetClientLogger()->info(__VA_ARGS__)
#define PN_TRACE(...)			::Pine::Log::GetClientLogger()->trace(__VA_ARGS__)