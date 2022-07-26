#include "GCEPCH.h"
#include "GCE/Renderer/SubTexture2D.h"

namespace GCE
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max) :
		m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::createFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize)
	{
		glm::vec2 min =
		{
			(coords.x * spriteSize.x) / texture->getWidth(),
			(coords.y * spriteSize.y) / texture->getHeight()
		};

		glm::vec2 max =
		{
			((coords.x + 1) * spriteSize.x) / texture->getWidth(),
			((coords.y + 1) * spriteSize.y) / texture->getHeight()
		};

		return createRef<SubTexture2D>(texture, min, max);
	}

}