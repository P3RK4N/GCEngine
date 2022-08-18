#include "GCEPCH.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace GCE
{

	void OpenGLRendererAPI::init()
	{
		GCE_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::setClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::drawIndexed(const GCE::Ref<VertexArray>& vertexArray, unsigned int indexCount)
	{
		unsigned int count = indexCount ? indexCount : vertexArray->getIndexBuffer()->getCount();
		vertexArray->bind();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::drawLines(const Ref<VertexArray>& vertexArray, unsigned int vertexCount)
	{
		vertexArray->bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRendererAPI::setLineWidth(float width)
	{
		glLineWidth(width);
	}

}