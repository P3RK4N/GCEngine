#include "GCE.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "GCE/Renderer/Texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public GCE::Layer
{
public:
	ExampleLayer() :	
		Layer("Example"),
		m_CameraController(16.0f/9.0f, true)
	{
		//VAO--------------------------------
		m_VertexArray.reset(GCE::VertexArray::create());

		//VBO---------------------------------------------------------------------
		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.25f, 0.6f, 0.3f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.3f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 0.5f, 0.7f, 0.1f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.7f, 0.1f, 0.6f, 0.0f, 1.0f
		};
		GCE::Ref<GCE::VertexBuffer> vb;
		vb.reset(GCE::VertexBuffer::create(vertices, sizeof(vertices)));
		vb->setLayout({ { GCE::ShaderDataType::Float3, "a_Position" }, { GCE::ShaderDataType::Float3, "a_Color" }, { GCE::ShaderDataType::Float2, "a_UV"} });
		m_VertexArray->addVertexBuffer(vb);

		//EBO------------------------------------------------------------------------------
		unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };
		GCE::Ref<GCE::IndexBuffer> ib;
		ib.reset(GCE::IndexBuffer::create(indices, sizeof(indices) / sizeof(indices[0])));
		m_VertexArray->setIndexBuffer(ib);

		//Shader---------------------------------------------------------------------------
		m_Shader = GCE::Shader::create("assets/shaders/texture.glsl");

		//Textures-------------------------------------------------------------------------
		m_Texture = GCE::Texture2D::create("assets/textures/gigachad.jpg");
		m_Glasses = GCE::Texture2D::create("assets/textures/glasses.png");

		std::dynamic_pointer_cast<GCE::OpenGLShader>(m_Shader)->bind();
		std::dynamic_pointer_cast<GCE::OpenGLShader>(m_Shader)->uploadUniformInt("u_Texture", 0);
	}

	virtual void onUpdate(GCE::Timestep ts) override
	{
		//Update
		m_CameraController.onUpdate(ts);
		m_Transform = glm::rotate(m_Transform, glm::radians(-1.0f / 144.0f * 90.0f), glm::vec3(0, 0, 1));

		//Render
		GCE::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
		GCE::RenderCommand::clear();

		GCE::Renderer::beginScene(m_CameraController.getCamera());

		std::dynamic_pointer_cast<GCE::OpenGLShader>(m_Shader)->bind();
		std::dynamic_pointer_cast<GCE::OpenGLShader>(m_Shader)->uploadUniformFloat3("u_Color", m_Color);

		m_Texture->bind();
		GCE::Renderer::submit(m_VertexArray, m_Shader, m_Transform);
		m_Glasses->bind();
		GCE::Renderer::submit(m_VertexArray, m_Shader, m_Transform);


		GCE::Renderer::endScene();
	}

	virtual void onEvent(GCE::Event& event) override
	{
		m_CameraController.onEvent(event);
	}

	virtual void onImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_Color));
		ImGui::End();
	}

private:
	GCE::ShaderLibrary m_ShaderLibrary;
	GCE::Ref<GCE::Shader> m_Shader;
	GCE::Ref<GCE::VertexArray> m_VertexArray;

	GCE::OrthographicCameraController m_CameraController;
	GCE::Ref<GCE::Texture2D> m_Texture, m_Glasses;
	glm::vec3 m_Color = { 0.0f, 0.0f, 0.0f };
	glm::mat4 m_Transform = glm::mat4(1.0f);
};

class SandBox : public GCE::Application
{
public:
	SandBox()
	{
		pushLayer(new ExampleLayer());
	}

	~SandBox()
	{

	}
};

GCE::Application* GCE::createApplication()
{
	return new SandBox();
}
