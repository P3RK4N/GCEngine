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

	public:
		static Shader* create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}