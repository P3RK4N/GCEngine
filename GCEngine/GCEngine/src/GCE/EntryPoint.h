#pragma once

#ifdef GCE_PLATFORM_WINDOWS

extern GCE::Application* GCE::createApplication();

int main(int argc, char** argv)
{
	std::cout << "GigaChad Engine\n";
	auto app = GCE::createApplication();
	app->Run();
	delete app;
}

#endif