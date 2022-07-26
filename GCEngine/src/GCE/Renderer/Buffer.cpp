#include "GCEPCH.h"
#include "GCE/Renderer/Buffer.h"

#include "GCE/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace GCE
{
	Ref<VertexBuffer> VertexBuffer::create(float* vertices, unsigned int size)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:		GCE_CORE_ASSERT(false, "Renderer API none is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return createRef<OpenGLVertexBuffer>(vertices, size);
		}

		GCE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::create(unsigned int size)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:		GCE_CORE_ASSERT(false, "Renderer API none is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return createRef<OpenGLVertexBuffer>(size);
		}

		GCE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::create(unsigned int* indices, unsigned int size)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:		GCE_CORE_ASSERT(false, "Renderer API none is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return createRef<OpenGLIndexBuffer>(indices, size);
		}

		GCE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}


}