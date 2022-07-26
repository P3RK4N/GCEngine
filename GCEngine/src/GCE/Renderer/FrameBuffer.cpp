#include "GCEPCH.h"
#include "GCE/Renderer/FrameBuffer.h"

#include "GCE/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace GCE
{
	Ref<FrameBuffer> FrameBuffer::create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:	GCE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return createRef<OpenGLFrameBuffer>(spec);
		}

		GCE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}