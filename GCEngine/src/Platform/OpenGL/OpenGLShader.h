#pragma once

#include "GCE/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace GCE
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& shaderName, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& path);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;
		
		virtual const std::string& getName() const override { return m_Name; }

		void uploadUniformInt(const std::string& name, int value);
		
		void uploadUniformFloat(const std::string& name, float value);
		void uploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void uploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void uploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		unsigned int m_RendererID;
		std::string m_Name;

	private:
		std::string readFile(const std::string& path);
		std::unordered_map<GLenum, std::string> preProcess(const std::string& source);
		void compile(const std::unordered_map<GLenum, std::string>& shaderSrc);
	};
}