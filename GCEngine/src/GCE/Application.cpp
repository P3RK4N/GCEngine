#include "GCEPCH.h"

#include "Application.h"

#include "GCE/Events/ApplicationEvent.h"
#include "GCE/Log.h"

namespace GCE
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		GCE::WindowResizeEvent e(1280, 720);
		GCE_TRACE(e);

		while (true)
		{

		}
	}
}