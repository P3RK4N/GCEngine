#include "GCEPCH.h"
#include "GCE/Renderer/Shader.h"

#include "GCE/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace GCE
{
	Ref<Shader> Shader::create(const std::string& shaderName, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		GCE_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(shaderName, vertexSrc, fragmentSrc);
		}

		GCE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::create(const std::string& path)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:		GCE_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(path);
		}

		GCE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::add(const Ref<Shader>& shader)
	{
		auto& name = shader->getName();
		add(name, shader);
	}

	void ShaderLibrary::add(const std::string& shaderName, const Ref<Shader>& shader)
	{
		GCE_CORE_ASSERT(!exists(shaderName), "Shader already exists!");
		m_Shaders[shaderName] = shader;
	}

	GCE::Ref<GCE::Shader> ShaderLibrary::load(const std::string& path)
	{
		auto shader = Shader::create(path);
		add(shader);
		return shader;
	}

	GCE::Ref<GCE::Shader> ShaderLibrary::load(const std::string& shaderName, const std::string& path)
	{
		auto shader = Shader::create(path);
		add(shaderName, shader);
		return shader;
	}

	GCE::Ref<GCE::Shader> ShaderLibrary::get(const std::string& shaderName)
	{
		GCE_CORE_ASSERT(exists(shaderName), "Shader already exists!");
		return m_Shaders[shaderName];
	}

	bool ShaderLibrary::exists(const std::string& shaderName) const
	{
		return m_Shaders.find(shaderName) != m_Shaders.end();
	}

}