#pragma once

#ifdef GCE_PLATFORM_WINDOWS

extern GCE::Application* GCE::createApplication();

int main(int argc, char** argv)
{
	GCE::Log::init();
	GCE_CORE_WARN("GigaChad Engine");
	GCE_INFO("Logging started!");

	auto app = GCE::createApplication();
	app->Run();
	delete app;
}

#endif