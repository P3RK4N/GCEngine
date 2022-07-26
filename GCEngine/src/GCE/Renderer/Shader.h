#pragma once

#include <string>
#include <glm/glm.hpp>

namespace GCE
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void setMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void setFloat(const std::string& name, float value) = 0;
		virtual void setFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void setFloat4(const std::string& name, const glm:: vec4 & value) = 0;
		virtual void setInt(const std::string& name, int value) = 0;
		virtual void setIntArray(const std::string& name, int* values, unsigned int count) = 0;

		virtual const std::string& getName() const = 0;

	public:
		static Ref<Shader> create(const std::string& shaderName, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> create(const std::string& path);
	};

	class ShaderLibrary
	{
	public:
		void add(const Ref<Shader>& shader);
		void add(const std::string& shaderName, const Ref<Shader>& shader);

		Ref<Shader> load(const std::string& path);
		Ref<Shader> load(const std::string& shaderName, const std::string& path);

		Ref<Shader> get(const std::string& shaderName);

		bool exists(const std::string& shaderName) const;

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}