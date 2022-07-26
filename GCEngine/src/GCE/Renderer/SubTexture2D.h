#pragma once
#include "GCE/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace GCE
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D>& getTexture() const { return m_Texture; }
		const glm::vec2* getTexCoords() const { return m_TexCoords; }

	public:
		static Ref<SubTexture2D> createFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize);

	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};
}