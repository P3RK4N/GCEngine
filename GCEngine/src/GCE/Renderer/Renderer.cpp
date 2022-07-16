#include "GCEPCH.h"
#include "GCE/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace GCE
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::beginScene(OrthographicCamera& camera)
	{
		m_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene()
	{

	}

	void Renderer::init()
	{
		RenderCommand::init();
	}

	void Renderer::onWindowResize(unsigned int width, unsigned int height)
	{
		RenderCommand::setViewport(0, 0, width, height);
	}

	void Renderer::submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transformMatrix)
	{
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_TransformMatrix", transformMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_ViewProjectionMatrix", m_SceneData->viewProjectionMatrix);

		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}