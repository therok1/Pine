#pragma once

#include "Pine/Core/PlatformDetection.h"

#include <memory>

#if defined(PN_PLATFORM_WINDOWS)
	#define PN_DEBUGBREAK() __debugbreak()
#elif defined(PN_PLATFORM_LINUX)
	#include <signal.h>
	#define PN_DEBUGBREAK() raise(SIGTRAP)
#else
	#error "Platform doesn't support debugbreak yet!"
#endif

#ifdef PN_DEBUG
	#define PN_ENABLE_ASSERTS
#endif

#ifdef PN_DIST
	#define PN_ENABLE_VERIFY
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
