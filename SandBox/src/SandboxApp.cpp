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
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
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
		std::string vert =
			R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Color;
			layout(location = 2) in vec2 a_UV;
			
			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_TransformMatrix;

			out vec3 v_Position;
			out vec3 v_Color;
			out vec2 v_UV;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				v_UV = a_UV;
				gl_Position = u_ViewProjectionMatrix * u_TransformMatrix * vec4(a_Position, 1.0);
			}
		)";

		std::string frag =
			R"(
			#version 330 core
			
			uniform vec3 u_Color;
			uniform sampler2D u_Texture;
			
			in vec3 v_Position;
			in vec3 v_Color;
			in vec2 v_UV;

			layout(location = 0) out vec4 color;
			
			void main()
			{
				color = texture(u_Texture, v_UV) * vec4(u_Color, 1.0);
			}
		)";

		m_Shader.reset(GCE::Shader::create(vert, frag));
		m_Texture = GCE::Texture2D::create("assets/textures/gigachad.jpg");
		m_Glasses = GCE::Texture2D::create("assets/textures/glasses.png");

		std::dynamic_pointer_cast<GCE::OpenGLShader>(m_Shader)->bind();
		std::dynamic_pointer_cast<GCE::OpenGLShader>(m_Shader)->uploadUniformInt("u_Texture", 0);
	}

	virtual void onUpdate(GCE::Timestep ts) override
	{
		inputMovement(ts);
		GCE::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
		GCE::RenderCommand::clear();

		m_Transform = glm::rotate(m_Transform, glm::radians(-1.0f), glm::vec3(0, 0, 1));


		GCE::Renderer::beginScene(m_Camera);

		std::dynamic_pointer_cast<GCE::OpenGLShader>(m_Shader)->bind();
		std::dynamic_pointer_cast<GCE::OpenGLShader>(m_Shader)->uploadUniformFloat3("u_Color", m_Color);

		m_Texture->bind();
		GCE::Renderer::submit(m_VertexArray, m_Shader, m_Transform);
		m_Glasses->bind();
		GCE::Renderer::submit(m_VertexArray, m_Shader, m_Transform);


		GCE::Renderer::endScene();
	}

	virtual void onImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_Color));
		ImGui::End();
	}

	void inputMovement(const GCE::Timestep& ts)
	{
		if (GCE::Input::isKeyPressed(GCE_KEY_LEFT))
			m_Camera.setPosition(m_Camera.getPosition() - glm::vec3(m_CameraSpeed * ts, 0.0f, 0.0f));
		if (GCE::Input::isKeyPressed(GCE_KEY_RIGHT))
			m_Camera.setPosition(m_Camera.getPosition() + glm::vec3(m_CameraSpeed * ts, 0.0f, 0.0f));
		if (GCE::Input::isKeyPressed(GCE_KEY_DOWN))
			m_Camera.setPosition(m_Camera.getPosition() - glm::vec3(0.0f, m_CameraSpeed * ts, 0.0f));
		if (GCE::Input::isKeyPressed(GCE_KEY_UP))
			m_Camera.setPosition(m_Camera.getPosition() + glm::vec3(0.0f, m_CameraSpeed * ts, 0.0f));
	}

private:
	GCE::Ref<GCE::Shader> m_Shader;
	GCE::Ref<GCE::VertexArray> m_VertexArray;

	GCE::OrthographicCamera m_Camera;
	GCE::Ref<GCE::Texture2D> m_Texture, m_Glasses;
	float m_CameraSpeed = 1.0f;
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
