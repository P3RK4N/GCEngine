#pragma once

#include "GCE/Renderer/Camera.h"
#include "GCE/Renderer/OrthographicCamera.h"
#include "GCE/Renderer/EditorCamera.h"

#include "GCE/Renderer/Texture.h"
#include "GCE/Renderer/SubTexture2D.h"

#include "GCE/Scene/Components.h"

#include <glm/glm.hpp>

namespace GCE
{
	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void beginScene(const Camera& camera, glm::mat4& transform);
		static void beginScene(const EditorCamera& camera);
		static void beginScene(const OrthographicCamera& orthographicCamera);

		static void endScene();

		static void startBatch();
		static void flush();

		//Primitives
		static void drawQuad
		(
			const glm::vec2& position,
			const glm::vec2& size, 
			const glm::vec4& color
		);
		static void drawQuad
		(
			const glm::vec3& position,
			const glm::vec2& size,
			const glm::vec4& color
		);
		static void drawQuad
		(
			const glm::vec2& position,
			const glm::vec2& size,
			const Ref<Texture2D>& texture,
			int textureScale = 1,
			const glm::vec4& color = glm::vec4(1.0f)
		);
		static void drawQuad
		(
			const glm::vec3& position,
			const glm::vec2& size,
			const Ref<Texture2D>& texture,
			int textureScale = 1,
			const glm::vec4& color = glm::vec4(1.0f)
		);
		static void drawQuad
		(
			const glm::vec2& position,
			const glm::vec2& size,
			const Ref<SubTexture2D>& subTexture,
			const glm::vec4& color = glm::vec4(1.0f)
		);
		static void drawQuad
		(
			const glm::vec3& position,
			const glm::vec2& size,
			const Ref<SubTexture2D>& subTexture,
			const glm::vec4& color = glm::vec4(1.0f)
		);
		static void drawQuad
		(
			const glm::mat4& transform, 
			const glm::vec4& color,
			int entityID = -1
		);
		static void drawQuad
		(
			const glm::mat4& transform,
			const Ref<Texture2D>& texture,
			int textureScale = 1,
			const glm::vec4& color = glm::vec4(1.0f),
			int entityID = -1
		);
		static void drawQuadRotated
		(
			const glm::vec2& position,
			const glm::vec2& size,
			float rotation,
			const glm::vec4& color
		);
		static void drawQuadRotated
		(
			const glm::vec3& position,
			const glm::vec2& size,
			float rotation,
			const glm::vec4& color
		);
		static void drawQuadRotated
		(
			const glm::vec2& position,
			const glm::vec2& size,
			float rotation,
			const Ref<Texture2D>& texture,
			int textureScale = 1,
			const glm::vec4& color = glm::vec4(1.0f)
		);
		static void drawQuadRotated
		(
			const glm::vec3& position,
			const glm::vec2& size,
			float rotation,
			const Ref<Texture2D>& texture,
			int textureScale = 1,
			const glm::vec4& color = glm::vec4(1.0f)
		);
		static void drawQuadRotated
		(
			const glm::vec2& position,
			const glm::vec2& size,
			float rotation,
			const Ref<SubTexture2D>& subTexture,
			const glm::vec4& color = glm::vec4(1.0f)
		);
		static void drawQuadRotated
		(
			const glm::vec3& position,
			const glm::vec2& size,
			float rotation,
			const Ref<SubTexture2D>& subTexture,
			const glm::vec4& color = glm::vec4(1.0f)
		);

		static void drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

		//Statistics
		struct Statistics
		{
			unsigned int drawCalls;
			unsigned int quadCount;

			unsigned int getTotalVertexCount() { return quadCount * 4; }
			unsigned int getTotalIndexCount() { return quadCount * 6; }
		};

		static Statistics getStats();
		static void resetStats();

	private:
		static void flushAndReset();
	};
}