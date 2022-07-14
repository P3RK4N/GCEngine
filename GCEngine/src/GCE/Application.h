#pragma once

#include "GCE/Core.h"

#include "GCE/Window.h"
#include "GCE/LayerStack.h"
#include "GCE/Events/Event.h"
#include "GCE/Events/ApplicationEvent.h"
#include "GCE/ImGui/ImGuiLayer.h"

#include "GCE/Core/Timestep.h"

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

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	Application* createApplication();
}

