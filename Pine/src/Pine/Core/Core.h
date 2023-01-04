#pragma once

#include <memory>

#ifdef PN_PLATFORM_WINDOWS
	#if PN_DYNAMIC_LINK
		#ifdef PN_BUILD_DLL
			#define PINE_API __declspec(dllexport)
		#else
			#define PINE_API __declspec(dllimport)
		#endif
	#else
		#define PINE_API
	#endif
#else
	#error Pine only supports Windows!
#endif

#ifdef PN_DEBUG
	#define PN_ENABLE_ASSERTS
#endif

#ifdef PN_ENABLE_ASSERTS
	#define PN_ASSERT(x, ...) { if (!(x)) { PN_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define PN_CORE_ASSERT(x, ...) { if (!(x)) { PN_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define PN_ASSERT(x, ...)
	#define PN_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define PN_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Pine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}