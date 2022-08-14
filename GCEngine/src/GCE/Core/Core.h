#pragma once

#include <memory>
#include "GCE/Core/PlatformDetection.h"

#ifdef GCE_PLATFORM_WINDOWS
	#if GCE_DYNAMIC_LINK
		#ifdef GCE_BUILD_DLL
			#define GCE_API __declspec(dllexport)
		#else 
			#define GCE_API __declspec(dllimport)
		#endif
	#else 
		#define GCE_API
	#endif
#else
	#error GC Engine only supports windows!
#endif

//#ifdef GCE_ENABLE_ASSERTS
//	#define GCE_ASSERT(x, ...) { if(!(x)) { GCE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
//	#define GCE_CORE_ASSERT(x, ...) { if(!(x)) { GCE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
//#else
//	#define GCE_ASSERT(x, ...)
//	#define GCE_CORE_ASSERT(x, ...)
//#endif 

#ifdef GCE_DEBUG

	#if defined(GCE_PLATFORM_WINDOWS)
		#define GCE_DEBUGBREAK() __debugbreak()

	#elif defined(GCE_PLATFORM_LINUX)
		#include <signal.h>
		#define GCE_DEBUGBREAK() raise(SIGTRAP)

	#else
		#error "Platform doesn't support debugbreak yet!"
	
	#endif
	
	#define GCE_ENABLE_ASSERTS

#else
	#define GCE_DEBUGBREAK()

#endif


#define HZ_EXPAND_MACRO(x) x
#define HZ_STRINGIFY_MACRO(x) #x
#define BIT(x) (1 << x)
#define GCE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1) 


namespace GCE
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args> 
	constexpr Scope<T> createScope(Args&& ... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> createRef(Args&& ... args) { return std::make_shared<T>(std::forward<Args>(args)...); }
}

#include "GCE/Core/Log.h"
#include "GCE/Core/Assert.h"