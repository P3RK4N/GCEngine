#include "GCEPCH.h"
#include "Application.h"

#include "GCE/Events/ApplicationEvent.h"

#include "glad/glad.h"

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
		m_LayerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

		GCE_CORE_TRACE(e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.end(); )
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

		glClearColor(1, 0, 1, 1);

		while (m_Running)
		{
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->onUpdate();

			m_Window->onUpdate();
		}
	}
}