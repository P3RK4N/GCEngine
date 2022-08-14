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
	struct ApplicationCommandLineArgs
	{
		int count = 0;
		char** args = nullptr;

		const char* operator[](int index) const
		{
			GCE_CORE_ASSERT(0 <= index && index < count, "Invalid index");

			return args[index];
		}
	};

	class GCE_API Application
	{
	public:
		Application(const std::string& name = "GCEngine", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void Run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		inline Window& getWindow() { return *m_Window; }

		ImGuiLayer* getImGuiLayer() { return m_ImGuiLayer; }

	public:
		inline static Application* get() { return s_Instance; }

		ApplicationCommandLineArgs getCommandLineArgs() const { return m_CommandLineArgs; }

	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
		Scope<Window> m_Window;

		bool m_Running = true;
		bool m_Minimized = false;
		
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	Application* createApplication(ApplicationCommandLineArgs args);
}

