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

		inline static void drawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->drawIndexed(vertexArray);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}