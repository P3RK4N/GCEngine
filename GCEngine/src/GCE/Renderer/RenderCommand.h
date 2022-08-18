#pragma once

#include "GCE/Renderer/RendererAPI.h"

namespace GCE
{
	class RenderCommand
	{
	public:
		inline static void init()
		{
			s_RendererAPI->init();
		}

		inline static void clear()
		{
			s_RendererAPI->clear();
		}

		inline static void setClearColor(const glm::vec4& color)
		{
			s_RendererAPI->setClearColor(color);
		}

		inline static void drawIndexed(const Ref<VertexArray>& vertexArray, unsigned int indexCount = 0)
		{
			s_RendererAPI->drawIndexed(vertexArray, indexCount);
		}

		inline static void drawLines(const Ref<VertexArray>& vertexArray, unsigned int vertexCount)
		{
			s_RendererAPI->drawLines(vertexArray, vertexCount);
		}

		inline static void setLineWidth(float width)
		{
			s_RendererAPI->setLineWidth(width);
		}

		inline static void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
		{
			s_RendererAPI->setViewport(x, y, width, height);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}