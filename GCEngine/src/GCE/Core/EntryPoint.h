#pragma once
#include "GCE/Core/Core.h"
#include "GCE/Core/Application.h"

#ifdef GCE_PLATFORM_WINDOWS

extern GCE::Application* GCE::createApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	GCE::Log::init();

	GCE_PROFILE_BEGIN_SESSION("Startup", "GCEProfile-Startup.json");
	auto app = GCE::createApplication({ argc, argv });
	GCE_PROFILE_END_SESSION();

	GCE_PROFILE_BEGIN_SESSION("Runtime", "GCEProfile-Runtime.json");
	app->Run();
	GCE_PROFILE_END_SESSION();

	GCE_PROFILE_BEGIN_SESSION("Runtime", "GCEProfile-Shutdown.json");
	delete app;
	GCE_PROFILE_END_SESSION();
}

#endif