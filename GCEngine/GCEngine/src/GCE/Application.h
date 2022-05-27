#pragma once

#include "Core.h"

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

