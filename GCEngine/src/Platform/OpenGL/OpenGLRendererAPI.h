#pragma once

#include "GCE/Renderer/RendererAPI.h"

namespace GCE
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void init() override;

		virtual void clear() override;
		virtual void setClearColor(const glm::vec4& color) override;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}