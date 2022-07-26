#pragma once

#ifdef GCE_PLATFORM_WINDOWS

extern GCE::Application* GCE::createApplication();

int main(int argc, char** argv)
{
	GCE::Log::init();

	GCE_PROFILE_BEGIN_SESSION("Startup", "GCEProfile-Startup.json");
	auto app = GCE::createApplication();
	GCE_PROFILE_END_SESSION();

	GCE_PROFILE_BEGIN_SESSION("Runtime", "GCEProfile-Runtime.json");
	app->Run();
	GCE_PROFILE_END_SESSION();

	GCE_PROFILE_BEGIN_SESSION("Runtime", "GCEProfile-Shutdown.json");
	delete app;
	GCE_PROFILE_END_SESSION();
}

#endif