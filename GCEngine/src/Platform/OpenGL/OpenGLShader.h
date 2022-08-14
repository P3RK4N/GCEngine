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


		virtual void setMat4(const std::string& name, const glm::mat4& value) override;
		virtual void setFloat(const std::string& name, float value) override;
		virtual void setFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void setFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void setInt(const std::string& name, int value) override;
		virtual void setIntArray(const std::string& name, int* values, unsigned int count) override;

		void uploadUniformInt(const std::string& name, int value);
		void uploadUniformIntArray(const std::string& name, int* values, unsigned int count);
		
		void uploadUniformFloat(const std::string& name, float value);
		void uploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void uploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void uploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		unsigned int m_RendererID;
		std::string m_Name;
		std::string m_FilePath;

		std::unordered_map<GLenum, std::vector<unsigned>> m_VulkanSPIRV;
		std::unordered_map<GLenum, std::vector<unsigned>> m_OpenGLSPIRV;
		std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;

	private:
		std::string readFile(const std::string& path);
		std::unordered_map<GLenum, std::string> preProcess(const std::string& source);

		void compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
		void compileOrGetOpenGLBinaries();
		void createProgram();
		void reflect(GLenum stage, const std::vector<uint32_t>& shaderData);
	};
}