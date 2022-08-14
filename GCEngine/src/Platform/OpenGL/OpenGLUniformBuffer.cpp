#include "GCEPCH.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace GCE
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(unsigned int size, unsigned int binding)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLUniformBuffer::setData(const void* data, unsigned int size, unsigned int offset)
	{
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}

}