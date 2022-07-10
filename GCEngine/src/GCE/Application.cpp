#include "GCEPCH.h"
#include "Application.h"

#include "GCE/Events/ApplicationEvent.h"
#include "GCE/Input.h"

#include "glad/glad.h"

namespace GCE
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}

		GCE_CORE_ASSERT(false, "Unknown shader data type");
		return 0;
	}

	Application::Application()
	{
		GCE_ASSERT(!s_Instance, "Application already exists");

		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

		m_ImGuiLayer = new ImGuiLayer();
		this->pushOverlay(m_ImGuiLayer);

		//VAO--------------------------------
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		//VBO---------------------------------------------------------------------
		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.25f, 0.6f, 0.3f,
			0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.3f,
			0.0f, 0.5f, 0.0f, 0.5f, 0.7f, 0.1f
		};

		m_VertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));

		m_VertexBuffer->setLayout
		(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Color" }
			}
		);

		unsigned int i = 0;
		for (const BufferElement& element : m_VertexBuffer->getLayout())
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer
			(
				i++,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				m_VertexBuffer->getLayout().getStride(),
				(const void*)element.offset
			);
		}

		//EBO------------------------------------------------------------------------------
		unsigned int indices[3] =
		{
			0, 1, 2
		};

		m_IndexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(indices[0])));

		//Shader---------------------------------------------------------------------------
		std::string vert =
		R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Color;
			
			out vec3 v_Position;
			out vec3 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string frag =
			R"(
			#version 330 core
			
			in vec3 v_Position;
			in vec3 v_Color;

			layout(location = 0) out vec4 color;
			
			void main()
			{
				color = vec4(v_Color, 1.0);
			}
		)";

		m_Shader.reset(new Shader(vert, frag));
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

		GCE_CORE_TRACE(e);

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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->onUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->onImGuiRender();
			m_ImGuiLayer->End();

			m_Window->onUpdate();
		}
	}
}