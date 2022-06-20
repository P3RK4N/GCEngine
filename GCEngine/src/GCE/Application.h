#pragma once

#include "GCE/Core.h"

#include "GCE/Window.h"
#include "GCE/LayerStack.h"
#include "GCE/Events/Event.h"
#include "GCE/Events/ApplicationEvent.h"

namespace GCE
{
	class GCE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		inline Window& getWindow() { return *m_Window; }
		inline static Application* get() { return s_Instance; }

	private:
		bool onWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	Application* createApplication();
}

