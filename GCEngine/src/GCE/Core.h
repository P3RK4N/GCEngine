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

#define BIT(x) (1 << x)