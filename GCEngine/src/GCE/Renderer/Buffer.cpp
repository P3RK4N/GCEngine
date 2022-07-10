#include "GCEPCH.h"
#include "GCE/Renderer/Buffer.h"

#include "GCE/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace GCE
{
	VertexBuffer* VertexBuffer::create(float* vertices, unsigned int size)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:		GCE_CORE_ASSERT(false, "Renderer API none is not supported"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLVertexBuffer(vertices, size);
		}

		GCE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::create(unsigned int* indices, unsigned int size)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:		GCE_CORE_ASSERT(false, "Renderer API none is not supported"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLIndexBuffer(indices, size);
		}

		GCE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}