#include "GCEPCH.h"
#include "GCE/Renderer/Texture.h"

#include "GCE/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace GCE
{
	GCE::Ref<GCE::Texture2D> Texture2D::create(const std::string& path)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		GCE_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return createRef<OpenGLTexture2D>(path);
		}

		GCE_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	GCE::Ref<Texture2D> Texture2D::create(unsigned int width, unsigned int height)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		GCE_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return createRef<OpenGLTexture2D>(width, height);
		}

		GCE_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}