#include "GCEPCH.h"
#include "Application.h"

#include "GCE/Events/ApplicationEvent.h"
#include "GCE/Renderer/Renderer.h"
#include "GCE/Input.h"

#include <GLFW/glfw3.h>

namespace GCE
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		GCE_ASSERT(!s_Instance, "Application already exists");

		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

		Renderer::init();

		m_ImGuiLayer = new ImGuiLayer();
		this->pushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{

	}

	void Application::pushLayer(Layer* layer)
	{
		m_LayerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer)
	{
		m_LayerStack.pushOverlay(layer);
		layer->onAttach();
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

		//GCE_CORE_TRACE(e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->onEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::Run()
	{
		GCE::WindowResizeEvent e(1280, 720);
		GCE_TRACE(e);

		while (m_Running)
		{
			float time = (float) glfwGetTime();	//Platform::getTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->onUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->onImGuiRender();
			m_ImGuiLayer->End();

			m_Window->onUpdate();
		}
	}
}