#pragma once

#include "Pine/Core/Core.h"
#include "Pine/Core/Log.h"

#include <filesystem>

#ifdef PN_ENABLE_ASSERTS
	#define PN_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { PN##type##ERROR(msg, __VA_ARGS__); PN_DEBUGBREAK(); } }
	#define PN_INTERNAL_ASSERT_WITH_MSG(type, check, ...) PN_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define PN_INTERNAL_ASSERT_NO_MSG(type, check) PN_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", PN_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define PN_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define PN_INTERNAL_ASSERT_GET_MACRO(...) PN_EXPAND_MACRO( PN_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, PN_INTERNAL_ASSERT_WITH_MSG, PN_INTERNAL_ASSERT_NO_MSG) )

	#define PN_ASSERT(...) PN_EXPAND_MACRO( PN_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define PN_CORE_ASSERT(...) PN_EXPAND_MACRO( PN_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define PN_ASSERT(...)
	#define PN_CORE_ASSERT(...)
#endif

#ifdef PN_ENABLE_VERIFY
	#define PN_INTERNAL_VERIFY_IMPL(type, check, msg, ...) { if(!(check)) { PN##type##ERROR(msg, __VA_ARGS__); PN_DEBUGBREAK(); } }
	#define PN_INTERNAL_VERIFY_WITH_MSG(type, check, ...) PN_INTERNAL_VERIFY_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define PN_INTERNAL_VERIFY_NO_MSG(type, check) PN_INTERNAL_VERIFY_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", PN_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define PN_INTERNAL_VERIFY_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define PN_INTERNAL_VERIFY_GET_MACRO(...) PN_EXPAND_MACRO( PN_INTERNAL_VERIFY_GET_MACRO_NAME(__VA_ARGS__, PN_INTERNAL_VERIFY_WITH_MSG, PN_INTERNAL_VERIFY_NO_MSG) )

	#define PN_VERIFY(...) PN_EXPAND_MACRO( PN_INTERNAL_VERIFY_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define PN_CORE_VERIFY(...) PN_EXPAND_MACRO( PN_INTERNAL_VERIFY_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define PN_VERIFY(...)
	#define PN_CORE_VERIFY(...)
#endif
