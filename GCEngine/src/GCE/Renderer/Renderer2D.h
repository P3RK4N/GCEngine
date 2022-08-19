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

		static void beginScene(const Camera& camera, const glm::mat4& transform);
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
			float textureScale = 1,
			const glm::vec4& color = glm::vec4(1.0f)
		);
		static void drawQuad
		(
			const glm::vec3& position,
			const glm::vec2& size,
			const Ref<Texture2D>& texture,
			float textureScale = 1,
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
			float textureScale = 1,
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
			float textureScale = 1,
			const glm::vec4& color = glm::vec4(1.0f)
		);
		static void drawQuadRotated
		(
			const glm::vec3& position,
			const glm::vec2& size,
			float rotation,
			const Ref<Texture2D>& texture,
			float textureScale = 1,
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

		static void drawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID = -1);
		static void drawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

		static void drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

		static void drawCircle(const glm::mat4 transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);

		static void drawLine(const glm::vec3& x, const glm::vec3& y, const glm::vec4& color, int entityID = -1);

		static float getLineWidth();
		static void setLineWidth(float lineWidth);

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