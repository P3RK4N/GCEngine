#pragma once

#include "GCE/Core.h"
#include "GCE/Events/Event.h"

namespace GCE
{
	class GCE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* createApplication();
}

