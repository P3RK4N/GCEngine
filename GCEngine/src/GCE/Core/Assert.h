#pragma once

#include "GCE/Core/Core.h"
#include "GCE/Core/Log.h"
#include <filesystem>

#ifdef GCE_ENABLE_ASSERTS
	#define GCE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { GCE##type##ERROR(msg, __VA_ARGS__); GCE_DEBUGBREAK(); } }
	#define GCE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) GCE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define GCE_INTERNAL_ASSERT_NO_MSG(type, check) GCE_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", GCE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define GCE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define GCE_INTERNAL_ASSERT_GET_MACRO(...) GCE_EXPAND_MACRO( GCE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, GCE_INTERNAL_ASSERT_WITH_MSG, GCE_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define GCE_ASSERT(...) GCE_EXPAND_MACRO( GCE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define GCE_CORE_ASSERT(...) GCE_EXPAND_MACRO( GCE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )

#else
	#define GCE_ASSERT(...)
	#define GCE_CORE_ASSERT(...)

#endif

#ifdef GCE_ENABLE_ASSERTS
	#define GCE_ASSERT(x, ...) { if(!(x)) { GCE_ERROR("Assertion failed: {0}", __VA_ARGS__); GCE_DEBUGBREAK(); } }
	#define GCE_CORE_ASSERT(x, ...) { if(!(x)) { GCE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); GCE_DEBUGBREAK(); } }
#else
	#define GCE_ASSERT(x, ...)
	#define GCE_CORE_ASSERT(x, ...)
#endif 