#pragma once

#include "Pine/Core/Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace Pine
{
	class Log
	{
	public:

		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:

		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;

	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
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
