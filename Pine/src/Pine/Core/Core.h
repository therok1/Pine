#pragma once

#include <memory>

#ifdef _WIN32
	#ifdef _WIN64
		#define PN_PLATFORM_WINDOWS
	#else
		#error "x86 builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_PHONE == 1
		#define PN_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define PN_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define PN_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define PN_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unknown platform!"
#endif

#ifdef PN_DEBUG
	#ifdef PN_PLATFORM_WINDOWS
		#define PN_DEBUGBREAK() __debugbreak()
	#elif defined(PN_PLATFORM_LINUX)
		#include <signal.h>
		#define PN_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define PN_ENABLE_ASSERTS
#else
	#define PN_DEBUGBREAK()
#endif

#ifdef PN_ENABLE_ASSERTS
	#define PN_ASSERT(x, ...) { if (!(x)) { PN_ERROR("Assertion failed: {0}", __VA_ARGS__); PN_DEBUGBREAK(); } }
	#define PN_CORE_ASSERT(x, ...) { if (!(x)) { PN_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); PN_DEBUGBREAK(); } }
#else
	#define PN_ASSERT(x, ...)
	#define PN_CORE_ASSERT(x, ...)
#endif

#define PN_EXPAND_MACRO(x) x
#define PN_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define PN_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Pine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "Pine/Core/Log.h"
#include "Pine/Core/Assert.h"
