#include "GCEPCH.h"
#include "GCE/Renderer/VertexArray.h"

#include "GCE/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace GCE
{
	VertexArray* VertexArray::create()
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		GCE_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return new OpenGLVertexArray();
		}

		GCE_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}
