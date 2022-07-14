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
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLTexture2D>(path);
		}

		GCE_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}