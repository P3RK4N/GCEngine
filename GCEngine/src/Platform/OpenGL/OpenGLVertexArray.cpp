#include "GCEPCH.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace GCE
{
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


	OpenGLVertexArray::OpenGLVertexArray()
	{
		GCE_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		GCE_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::bind() const
	{
		GCE_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::unbind() const
	{
		GCE_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const GCE::Ref<VertexBuffer>& vertexBuffer)
	{
		GCE_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		vertexBuffer->bind();

		GCE_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex buffer has no layout");

		unsigned int i = 0;
		for (const BufferElement& element : vertexBuffer->getLayout())
		{
			glEnableVertexAttribArray(i);

			switch (element.type)
			{
				case ShaderDataType::Float:		
				case ShaderDataType::Float2:	
				case ShaderDataType::Float3:	
				case ShaderDataType::Float4:
				{
					glVertexAttribPointer
					(
						i++,
						element.getComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						vertexBuffer->getLayout().getStride(),
						(const void*)element.offset
					);
					break;
				}

				case ShaderDataType::Int:		
				case ShaderDataType::Int2:		
				case ShaderDataType::Int3:		
				case ShaderDataType::Int4:		
				case ShaderDataType::Bool:
				{
					glVertexAttribIPointer
					(
						i++,
						element.getComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.type),
						vertexBuffer->getLayout().getStride(),
						(const void*)element.offset
					);
					break;
				}

				case ShaderDataType::Mat3:		
				case ShaderDataType::Mat4:
					break;
			}
			
		}
		
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		GCE_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->bind();

		m_IndexBuffer = indexBuffer;
	}
}

