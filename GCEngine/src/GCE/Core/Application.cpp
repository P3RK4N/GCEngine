#include "GCEPCH.h"
#include "GCE/Core/Application.h"

#include "GCE/Events/ApplicationEvent.h"
#include "GCE/Renderer/Renderer.h"
#include "GCE/Core/Input.h"

#include <GLFW/glfw3.h>

namespace GCE
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		GCE_PROFILE_FUNCTION();

		GCE_ASSERT(!s_Instance, "Application already exists");

		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

		Renderer::init();

		m_ImGuiLayer = new ImGuiLayer();
		this->pushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		GCE_PROFILE_FUNCTION();

	}

	void Application::pushLayer(Layer* layer)
	{
		GCE_PROFILE_FUNCTION();

		m_LayerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer)
	{
		GCE_PROFILE_FUNCTION();

		m_LayerStack.pushOverlay(layer);
		layer->onAttach();
	}

	void Application::onEvent(Event& e)
	{
		GCE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::onWindowResize));

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

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		GCE_PROFILE_FUNCTION();

		if (e.getWidth() == 0 || e.getHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		
		Renderer::onWindowResize(e.getWidth(), e.getHeight());

		m_Minimized = false;
		return false;
	}

	void Application::Run()
	{
		GCE_PROFILE_SCOPE("RunLoop");

		while (m_Running)
		{
			float time = (float) glfwGetTime();	//Platform::getTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				GCE_PROFILE_SCOPE("LayerStack onUpdate");

				for (Layer* layer : m_LayerStack)
					layer->onUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			{
				GCE_PROFILE_SCOPE("ImGUI LayerStack onUpdate");

				for (Layer* layer : m_LayerStack)
					layer->onImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->onUpdate();
		}
	}
}