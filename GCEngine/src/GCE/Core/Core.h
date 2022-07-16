#pragma once

#include <memory>

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

#ifdef GCE_DEBUG
	#define GCE_ENABLE_ASSERTS
#endif

#ifdef GCE_ENABLE_ASSERTS
	#define GCE_ASSERT(x, ...) { if(!(x)) { GCE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define GCE_CORE_ASSERT(x, ...) { if(!(x)) { GCE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define GCE_ASSERT(x, ...)
	#define GCE_CORE_ASSERT(x, ...)
#endif 


#define BIT(x) (1 << x)

#define GCE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1) 


namespace GCE
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}