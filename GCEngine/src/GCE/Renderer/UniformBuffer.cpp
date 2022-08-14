#include "GCEPCH.h"
#include "GCE/Renderer/UniformBuffer.h"

#include "GCE/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace GCE {

	Ref<UniformBuffer> UniformBuffer::create(unsigned int size, unsigned int binding)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:    GCE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLUniformBuffer>(size, binding);
		}

		GCE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}