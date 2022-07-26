#pragma once

#include "GCE/Core/Core.h"

#include "GCE/Core/Window.h"
#include "GCE/Core/LayerStack.h"
#include "GCE/Events/Event.h"
#include "GCE/Events/ApplicationEvent.h"
#include "GCE/ImGui/ImGuiLayer.h"

#include "GCE/Core/Timestep.h"

namespace GCE
{
	class GCE_API Application
	{
	public:
		Application(const std::string& name = "GCEngine");
		virtual ~Application();

		void Run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		inline Window& getWindow() { return *m_Window; }
		inline static Application* get() { return s_Instance; }

	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_Window;

		bool m_Running = true;
		bool m_Minimized = false;
		
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	Application* createApplication();
}

