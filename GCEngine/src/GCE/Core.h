#pragma once

#ifdef GCE_PLATFORM_WINDOWS
	#ifdef GCE_BUILD_DLL
		#define GCE_API __declspec(dllexport)
	#else 
		#define GCE_API __declspec(dllimport)
	#endif
#else
	#error GC Engine only supports windows!
#endif

#ifdef GCE_ENABLE_ASSERTS
	#define GCE_ASSERT(x, ...) { if(!(x)) { GCE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define GCE_CORE_ASSERT(x, ...) { if(!(x)) { GCE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define GCE_ASSERT(x, ...)
	#define GCE_CORE_ASSERT(x, ...)
#endif 


#define BIT(x) (1 << x)