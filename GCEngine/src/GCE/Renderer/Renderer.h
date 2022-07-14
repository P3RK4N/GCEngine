#pragma once

#include "GCE/Renderer/RenderCommand.h"
#include "GCE/Renderer/OrthographicCamera.h"
#include "GCE/Renderer/Shader.h"

//Projection * View * Transform
//Transform = translationMat * scaleMat

namespace GCE
{
	class Renderer
	{
	public:
		static void beginScene(OrthographicCamera& camera);
		static void endScene();

		static void init();

		static void submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transformMatrix = glm::mat4(1.0f));

		inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}