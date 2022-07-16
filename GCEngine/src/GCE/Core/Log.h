#pragma once

#include "GCE/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace GCE
{
	class GCE_API Log
	{
	public:
		Log();
		~Log();

		static void init();
		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define GCE_CORE_TRACE(...)		::GCE::Log::getCoreLogger()->trace(__VA_ARGS__)
#define GCE_CORE_INFO(...)		::GCE::Log::getCoreLogger()->info(__VA_ARGS__)
#define GCE_CORE_WARN(...)		::GCE::Log::getCoreLogger()->warn(__VA_ARGS__)
#define GCE_CORE_ERROR(...)		::GCE::Log::getCoreLogger()->error(__VA_ARGS__)
#define GCE_CORE_FATAL(...)		::GCE::Log::getCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define GCE_TRACE(...)		::GCE::Log::getClientLogger()->trace(__VA_ARGS__)
#define GCE_INFO(...)		::GCE::Log::getClientLogger()->info(__VA_ARGS__)
#define GCE_WARN(...)		::GCE::Log::getClientLogger()->warn(__VA_ARGS__)
#define GCE_ERROR(...)		::GCE::Log::getClientLogger()->error(__VA_ARGS__)
#define GCE_FATAL(...)		::GCE::Log::getClientLogger()->fatal(__VA_ARGS__)