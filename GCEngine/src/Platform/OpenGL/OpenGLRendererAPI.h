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

		virtual void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray, unsigned int indexCount = 0) override;
		virtual void drawLines(const Ref<VertexArray>& vertexArray, unsigned int vertexCount) override;

		virtual void setLineWidth(float width) override;
	};
}