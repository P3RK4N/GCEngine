#include "GCEPCH.h"
#include "GCE/Renderer/Renderer2D.h"

#include "GCE/Renderer/VertexArray.h"
#include "GCE/Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "GCE/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GCE
{
	struct QuadVertex
	{
		glm::vec3 pos;
		glm::vec4 col;
		glm::vec2 uv;
		float texIndex;
		float textureScale;
	};

	struct Renderer2DData
	{
		static const unsigned int maxQuads = 10000;
		static const unsigned int maxVertices = maxQuads * 4;
		static const unsigned int maxIndices = maxQuads * 6;
		static const unsigned int maxTextureSlots = 32; //RenderCaps TODO

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTex;

		unsigned int quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		unsigned int textureSlotIndex = 1;

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::init()
	{
		GCE_PROFILE_FUNCTION();

		s_Data.quadVertexArray = VertexArray::create();

		//VBO
		s_Data.quadVertexBuffer = VertexBuffer::create(s_Data.maxVertices * sizeof(QuadVertex));
		s_Data.quadVertexBuffer->setLayout
		({ 
			{ ShaderDataType::Float3, "a_position" }, 
			{ ShaderDataType::Float4, "a_Color" }, 
			{ ShaderDataType::Float2, "a_UV" },
			{ ShaderDataType::Float, "a_TextureIndex" },
			{ ShaderDataType::Float, "a_TextureScale" }
		});

		s_Data.quadVertexArray->addVertexBuffer(s_Data.quadVertexBuffer);

		s_Data.quadVertexBufferBase = new QuadVertex[s_Data.maxVertices];

		unsigned int* quadIndices = new unsigned int[s_Data.maxIndices];

		unsigned int offset = 0;	
		for (unsigned int i = 0; i < s_Data.maxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		//EBO
		Ref<IndexBuffer> quadIB = IndexBuffer::create(quadIndices, s_Data.maxIndices);
		s_Data.quadVertexArray->setIndexBuffer(quadIB);
		//----------
		delete[] quadIndices;

		//Texture
		s_Data.whiteTex = Texture2D::create(1, 1);
		unsigned int whiteTextureData = 0xffffffff;
		s_Data.whiteTex->setData(&whiteTextureData, sizeof(unsigned int));


		int  samplers[s_Data.maxTextureSlots];
		for (unsigned int i = 0; i < s_Data.maxTextureSlots; i++)
			samplers[i] = i;

		//Shader
		s_Data.textureShader = Shader::create("assets/shaders/texture.glsl");
		s_Data.textureShader->bind();
		s_Data.textureShader->setIntArray("u_Textures", samplers, s_Data.maxTextureSlots);


		s_Data.textureSlots[0] = s_Data.whiteTex;

		s_Data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::shutdown()
	{
		GCE_PROFILE_FUNCTION();

		delete[] s_Data.quadVertexBufferBase;
	}

	void Renderer2D::beginScene(const OrthographicCamera& orthographicCamera)
	{
		GCE_PROFILE_FUNCTION();

		s_Data.textureShader->bind();
		s_Data.textureShader->setMat4("u_ViewProjectionMatrix", orthographicCamera.getViewProjectionMatrix());

		s_Data.quadIndexCount = 0;
		s_Data.textureSlotIndex = 1;
		s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;
	}

	void Renderer2D::beginScene(const Camera& camera, glm::mat4& transform)
	{
		GCE_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.getProjection() * glm::inverse(transform);

		s_Data.textureShader->bind();
		s_Data.textureShader->setMat4("u_ViewProjectionMatrix", viewProj);

		startBatch();
	}

	void Renderer2D::beginScene(const EditorCamera& camera)
	{
		GCE_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.getViewProjection();

		s_Data.textureShader->bind();
		s_Data.textureShader->setMat4("u_ViewProjectionMatrix", viewProj);

		startBatch();
	}

	void Renderer2D::startBatch()
	{
		s_Data.quadIndexCount = 0;
		s_Data.textureSlotIndex = 1;
		s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;
	}

	void Renderer2D::endScene()
	{
		GCE_PROFILE_FUNCTION();

		unsigned int dataSize = (unsigned int)((uint8_t*)s_Data.quadVertexBufferPtr - (uint8_t*)s_Data.quadVertexBufferBase);
		s_Data.quadVertexBuffer->setData(s_Data.quadVertexBufferBase, dataSize);

		flush();
	}

	void Renderer2D::flush()
	{
		if(!s_Data.quadIndexCount)
			return;

		for (unsigned int i = 0; i < s_Data.textureSlotIndex; i++)
		{
			s_Data.textureSlots[i]->bind(i);
		}

		RenderCommand::drawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
		s_Data.stats.drawCalls++;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		GCE_PROFILE_FUNCTION();

		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		constexpr int quadVertexCount = 4;
		constexpr float textureIndex = 0.0f;
		constexpr float textureScale = 1.0f;
		constexpr glm::vec2 texCoords[quadVertexCount] = { { 0.0f, 0.0f}, { 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(size, 1.0f));

		for (int i = 0; i < quadVertexCount; i++)
		{
			s_Data.quadVertexBufferPtr->pos = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->col = color;
			s_Data.quadVertexBufferPtr->uv = texCoords[i];
			s_Data.quadVertexBufferPtr->texIndex = textureIndex;
			s_Data.quadVertexBufferPtr->textureScale = textureScale;
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
		s_Data.stats.quadCount++;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, int textureScale, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, textureScale, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, int textureScale, const glm::vec4& color)
	{
		GCE_PROFILE_FUNCTION();

		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		constexpr int quadVertexCount = 4;
		constexpr glm::vec2 texCoords[quadVertexCount] = { { 0.0f, 0.0f}, { 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		float textureIndex = 0.0f;

		for (unsigned int i = 0; i < s_Data.textureSlotIndex; i++)
		{
			if (*s_Data.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.textureSlotIndex == 32)
				flushAndReset();

			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
			s_Data.textureSlotIndex++;
		}

		glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(size, 1.0f));

		for (int i = 0; i < quadVertexCount; i++)
		{
			s_Data.quadVertexBufferPtr->pos = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->col = color;
			s_Data.quadVertexBufferPtr->uv = texCoords[i];
			s_Data.quadVertexBufferPtr->texIndex = textureIndex;
			s_Data.quadVertexBufferPtr->textureScale = textureScale;
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
		s_Data.stats.quadCount++;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0 }, size, subTexture, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color)
	{
		GCE_PROFILE_FUNCTION();

		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		constexpr int quadVertexCount = 4;
		constexpr float textureScale = 1.0f;
		const glm::vec2* texCoords = subTexture->getTexCoords();
		const Ref<Texture2D> texture = subTexture->getTexture();

		float textureIndex = 0.0f;

		for (unsigned int i = 0; i < s_Data.textureSlotIndex; i++)
		{
			if (*s_Data.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.textureSlotIndex == 32)
				flushAndReset();

			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
			s_Data.textureSlotIndex++;
		}

		glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(size, 1.0f));

		for (int i = 0; i < quadVertexCount; i++)
		{
			s_Data.quadVertexBufferPtr->pos = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->col = color;
			s_Data.quadVertexBufferPtr->uv = texCoords[i];
			s_Data.quadVertexBufferPtr->texIndex = textureIndex;
			s_Data.quadVertexBufferPtr->textureScale = textureScale;
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
		s_Data.stats.quadCount++;
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		GCE_PROFILE_FUNCTION();

		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		constexpr int quadVertexCount = 4;
		constexpr float textureIndex = 0.0f;
		constexpr float textureScale = 1.0f;
		constexpr glm::vec2 texCoords[quadVertexCount] = { { 0.0f, 0.0f}, { 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		for (int i = 0; i < quadVertexCount; i++)
		{
			s_Data.quadVertexBufferPtr->pos = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->col = color;
			s_Data.quadVertexBufferPtr->uv = texCoords[i];
			s_Data.quadVertexBufferPtr->texIndex = textureIndex;
			s_Data.quadVertexBufferPtr->textureScale = textureScale;
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
		s_Data.stats.quadCount++;
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, int textureScale, const glm::vec4& color)
	{
		GCE_PROFILE_FUNCTION();

		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		constexpr int quadVertexCount = 4;
		constexpr glm::vec2 texCoords[quadVertexCount] = { { 0.0f, 0.0f}, { 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		float textureIndex = 0.0f;

		for (unsigned int i = 0; i < s_Data.textureSlotIndex; i++)
		{
			if (*s_Data.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.textureSlotIndex == 32)
				flushAndReset();

			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
			s_Data.textureSlotIndex++;
		}

		for (int i = 0; i < quadVertexCount; i++)
		{
			s_Data.quadVertexBufferPtr->pos = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->col = color;
			s_Data.quadVertexBufferPtr->uv = texCoords[i];
			s_Data.quadVertexBufferPtr->texIndex = textureIndex;
			s_Data.quadVertexBufferPtr->textureScale = textureScale;
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
		s_Data.stats.quadCount++;
	}

	void Renderer2D::drawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		drawQuadRotated({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::drawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		GCE_PROFILE_FUNCTION();

		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		constexpr int quadVertexCount = 4;
		constexpr float textureIndex = 0.0f;
		constexpr float textureScale = 1.0f;
		constexpr glm::vec2 texCoords[quadVertexCount] = { { 0.0f, 0.0f}, { 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), position), -rotation, { 0,0,1 }), glm::vec3(size, 1.0f));

		for (int i = 0; i < quadVertexCount; i++)
		{
			s_Data.quadVertexBufferPtr->pos = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->col = color;
			s_Data.quadVertexBufferPtr->uv = texCoords[i];
			s_Data.quadVertexBufferPtr->texIndex = textureIndex;
			s_Data.quadVertexBufferPtr->textureScale = textureScale;
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
		s_Data.stats.quadCount++;
	}

	void Renderer2D::drawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, int textureScale, const glm::vec4& color)
	{
		drawQuadRotated({ position.x, position.y, 0.0f }, size, rotation, texture, textureScale, color);
	}

	void Renderer2D::drawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, int textureScale, const glm::vec4& color)
	{
		GCE_PROFILE_FUNCTION();

		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		constexpr int quadVertexCount = 4;
		constexpr glm::vec2 texCoords[quadVertexCount] = { { 0.0f, 0.0f}, { 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		float textureIndex = 0.0f;

		for (unsigned int i = 0; i < s_Data.textureSlotIndex; i++)
		{
			if (*s_Data.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.textureSlotIndex == 32)
				flushAndReset();

			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
			s_Data.textureSlotIndex++;
		}

		glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), position), -rotation, { 0,0,1 }), glm::vec3(size, 1.0f));

		for (int i = 0; i < quadVertexCount; i++)
		{
			s_Data.quadVertexBufferPtr->pos = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->col = color;
			s_Data.quadVertexBufferPtr->uv = texCoords[i];
			s_Data.quadVertexBufferPtr->texIndex = textureIndex;
			s_Data.quadVertexBufferPtr->textureScale = textureScale;
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
		s_Data.stats.quadCount++;
	}

	void Renderer2D::drawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, const glm::vec4& color)
	{
		drawQuadRotated({ position.x, position.y, 0 }, size, rotation, subTexture, color);

	}

	void Renderer2D::drawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, const glm::vec4& color)
	{
		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		constexpr int quadVertexCount = 4;
		constexpr float textureScale = 1.0f;
		const glm::vec2* texCoords = subTexture->getTexCoords();
		const Ref<Texture2D> texture = subTexture->getTexture();

		float textureIndex = 0.0f;

		for (unsigned int i = 0; i < s_Data.textureSlotIndex; i++)
		{
			if (*s_Data.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.textureSlotIndex == 32)
				flushAndReset();

			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
			s_Data.textureSlotIndex++;
		}

		glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), position), -rotation, { 0,0,1 }), glm::vec3(size, 1.0f));

		for (int i = 0; i < quadVertexCount; i++)
		{
			s_Data.quadVertexBufferPtr->pos = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->col = color;
			s_Data.quadVertexBufferPtr->uv = texCoords[i];
			s_Data.quadVertexBufferPtr->texIndex = textureIndex;
			s_Data.quadVertexBufferPtr->textureScale = textureScale;
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
		s_Data.stats.quadCount++;
	}

	Renderer2D::Statistics Renderer2D::getStats()
	{
		return s_Data.stats;
	}

	void Renderer2D::resetStats()
	{
		memset(&s_Data.stats, 0, sizeof(Renderer2D::Statistics));
	}

	void Renderer2D::flushAndReset()
	{
		endScene();

		startBatch();
	}

}