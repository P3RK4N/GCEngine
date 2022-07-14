#include "GCEPCH.h"
#include "GCE/Renderer/Shader.h"

#include "GCE/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace GCE
{
	Shader* Shader::create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		GCE_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			case RendererAPI::API::OpenGL:		return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		GCE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}