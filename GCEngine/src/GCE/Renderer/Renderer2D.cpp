#include "GCEPCH.h"
#include "GCE/Renderer/Renderer2D.h"

#include "GCE/Renderer/VertexArray.h"
#include "GCE/Renderer/Shader.h"
#include "GCE/Renderer/UniformBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "GCE/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GCE
{
	struct QuadVertex
	{
		glm::vec3 pos;
		glm::vec4 col;
		glm::vec2 uv;
		float texIndex;
		float textureScale;

		//Editor only
		int entityID;
	};

	struct CircleVertex
	{
		glm::vec3 worldPosition;
		glm::vec3 localPosition;
		glm::vec4 color;
		float thickness;
		float fade;

		//Editor only
		int entityID;
	};

	struct LineVertex
	{
		glm::vec3 position;
		glm::vec4 color;

		//Editor only
		int entityID;
	};

	struct Renderer2DData
	{
		static const unsigned int maxQuads = 10000;
		static const unsigned int maxVertices = maxQuads * 4;
		static const unsigned int maxIndices = maxQuads * 6;
		static const unsigned int maxTextureSlots = 32; //RenderCaps TODO
		glm::vec4 quadVertexPositions[4];

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> quadShader;

		unsigned int quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;


		Ref<VertexArray> circleVertexArray;
		Ref<VertexBuffer> circleVertexBuffer;
		Ref<Shader> circleShader;

		unsigned int circleIndexCount = 0;
		CircleVertex* circleVertexBufferBase = nullptr;
		CircleVertex* circleVertexBufferPtr = nullptr;


		Ref<VertexArray> lineVertexArray;
		Ref<VertexBuffer> lineVertexBuffer;
		Ref<Shader> lineShader;

		unsigned int lineVertexCount = 0;
		LineVertex* lineVertexBufferBase = nullptr;
		LineVertex* lineVertexBufferPtr = nullptr;

		float lineWidth = 2.0f;


		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		Ref<Texture2D> whiteTex;
		unsigned int textureSlotIndex = 1;

		Renderer2D::Statistics stats;

		struct CameraData
		{
			glm::mat4 viewProjection;
		};

		CameraData cameraBuffer;
		Ref<UniformBuffer> cameraUniformBuffer;
	};

	static Renderer2DData s_Data;

	void Renderer2D::init()
	{
		GCE_PROFILE_FUNCTION();

		unsigned int* indices = new unsigned int[s_Data.maxIndices];

		unsigned int offset = 0;	
		for (unsigned int i = 0; i < s_Data.maxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		//----------------------------------------------------------------------------------------------------
		s_Data.quadVertexArray = VertexArray::create();

		//VBO
		s_Data.quadVertexBuffer = VertexBuffer::create(s_Data.maxVertices * sizeof(QuadVertex));
		s_Data.quadVertexBuffer->setLayout
		({ 
			{ ShaderDataType::Float3, "a_Position" }, 
			{ ShaderDataType::Float4, "a_Color" }, 
			{ ShaderDataType::Float2, "a_UV" },
			{ ShaderDataType::Float,  "a_TexIndex" },
			{ ShaderDataType::Float,  "a_TextureScale" },
			{ ShaderDataType::Int,	  "a_EntityID" }
		});

		s_Data.quadVertexArray->addVertexBuffer(s_Data.quadVertexBuffer);

		s_Data.quadVertexBufferBase = new QuadVertex[s_Data.maxVertices];

		//EBO
		Ref<IndexBuffer> quadIB = IndexBuffer::create(indices, s_Data.maxIndices);
		s_Data.quadVertexArray->setIndexBuffer(quadIB);
		//-------------------------------------------------------------------------------------------------------

		s_Data.circleVertexArray = VertexArray::create();

		//VBO
		s_Data.circleVertexBuffer = VertexBuffer::create(s_Data.maxVertices * sizeof(CircleVertex));
		s_Data.circleVertexBuffer->setLayout
		({ 
			{ ShaderDataType::Float3,	"a_WorldPosition" }, 
			{ ShaderDataType::Float3,	"a_LocalPosition" }, 
			{ ShaderDataType::Float4,	"a_Color" }, 
			{ ShaderDataType::Float,	"a_Thickness" },
			{ ShaderDataType::Float,	"a_Fade" },
			{ ShaderDataType::Int,		"a_EntityID" }
		});

		s_Data.circleVertexArray->addVertexBuffer(s_Data.circleVertexBuffer);
		s_Data.circleVertexArray->setIndexBuffer(quadIB);
		s_Data.circleVertexBufferBase = new CircleVertex[s_Data.maxVertices];
		//-------------------------------------------------------------------------------------------

		s_Data.lineVertexArray = VertexArray::create();
		//VBO
		s_Data.lineVertexBuffer = VertexBuffer::create(s_Data.maxVertices * sizeof(LineVertex));
		s_Data.lineVertexBuffer->setLayout
		({ 
			{ ShaderDataType::Float3,	"a_Position" }, 
			{ ShaderDataType::Float4,	"a_Color" },
			{ ShaderDataType::Int,		"a_EntityID" }
		});

		s_Data.lineVertexArray->addVertexBuffer(s_Data.lineVertexBuffer);
		s_Data.lineVertexBufferBase = new LineVertex[s_Data.maxVertices];
		//-------------------------------------------------------------------------------------------
		delete[] indices;

		//Texture
		s_Data.whiteTex = Texture2D::create(1, 1);
		unsigned int whiteTextureData = 0xffffffff;
		s_Data.whiteTex->setData(&whiteTextureData, sizeof(unsigned int));


		int  samplers[s_Data.maxTextureSlots];
		for (unsigned int i = 0; i < s_Data.maxTextureSlots; i++)
			samplers[i] = i;

		//Shader
		s_Data.quadShader = Shader::create("assets/shaders/Renderer2D_Quad.glsl");
		s_Data.circleShader = Shader::create("assets/shaders/Renderer2D_Circle.glsl");
		s_Data.lineShader = Shader::create("assets/shaders/Renderer2D_Line.glsl");

		s_Data.textureSlots[0] = s_Data.whiteTex;

		s_Data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		s_Data.cameraUniformBuffer = UniformBuffer::create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::shutdown()
	{
		GCE_PROFILE_FUNCTION();

		delete[] s_Data.quadVertexBufferBase;
		delete[] s_Data.circleVertexBufferBase;
	}

	void Renderer2D::beginScene(const OrthographicCamera& orthographicCamera)
	{
		GCE_PROFILE_FUNCTION();

		s_Data.quadShader->bind();
		s_Data.quadShader->setMat4("u_ViewProjectionMatrix", orthographicCamera.getViewProjectionMatrix());

		startBatch();
	}

	void Renderer2D::beginScene(const Camera& camera, const glm::mat4& transform)
	{
		GCE_PROFILE_FUNCTION();

		s_Data.cameraBuffer.viewProjection = camera.getProjection() * glm::inverse(transform);
		s_Data.cameraUniformBuffer->setData(&s_Data.cameraBuffer, sizeof(Renderer2DData::CameraData));

		startBatch();
	}

	void Renderer2D::beginScene(const EditorCamera& camera)
	{
		GCE_PROFILE_FUNCTION();

		s_Data.cameraBuffer.viewProjection = camera.getViewProjection();
		s_Data.cameraUniformBuffer->setData(&s_Data.cameraBuffer, sizeof(Renderer2DData::CameraData));

		startBatch();
	}

	void Renderer2D::startBatch()
	{
		s_Data.quadIndexCount = 0;
		s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;

		s_Data.circleIndexCount = 0;
		s_Data.circleVertexBufferPtr = s_Data.circleVertexBufferBase;

		s_Data.lineVertexCount = 0;
		s_Data.lineVertexBufferPtr = s_Data.lineVertexBufferBase;

		s_Data.textureSlotIndex = 1;
	}

	void Renderer2D::endScene()
	{
		GCE_PROFILE_FUNCTION();

		flush();
	}

	void Renderer2D::flush()
	{
		if (s_Data.quadIndexCount)
		{
			unsigned int dataSize = (unsigned int)((uint8_t*)s_Data.quadVertexBufferPtr - (uint8_t*)s_Data.quadVertexBufferBase);
			s_Data.quadVertexBuffer->setData(s_Data.quadVertexBufferBase, dataSize);

			for (unsigned int i = 0; i < s_Data.textureSlotIndex; i++)
			{
				s_Data.textureSlots[i]->bind(i);
			}

			s_Data.quadShader->bind();
			RenderCommand::drawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
			s_Data.stats.drawCalls++;	
		}

		if (s_Data.circleIndexCount)
		{
			unsigned int dataSize = (unsigned int)((uint8_t*)s_Data.circleVertexBufferPtr - (uint8_t*)s_Data.circleVertexBufferBase);
			s_Data.circleVertexBuffer->setData(s_Data.circleVertexBufferBase, dataSize);

			s_Data.circleShader->bind();
			RenderCommand::drawIndexed(s_Data.circleVertexArray, s_Data.circleIndexCount);
			s_Data.stats.drawCalls++;	
		}

		if (s_Data.lineVertexCount > 1)
		{
			unsigned int dataSize = (unsigned int)((uint8_t*)s_Data.lineVertexBufferPtr - (uint8_t*)s_Data.lineVertexBufferBase);
			s_Data.lineVertexBuffer->setData(s_Data.lineVertexBufferBase, dataSize);

			s_Data.lineShader->bind();
			RenderCommand::setLineWidth(s_Data.lineWidth);
			RenderCommand::drawLines(s_Data.lineVertexArray, s_Data.lineVertexCount);
			s_Data.stats.drawCalls++;	
		}
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

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float textureScale, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, textureScale, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float textureScale, const glm::vec4& color)
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

	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
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
			s_Data.quadVertexBufferPtr->entityID = entityID;
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
		s_Data.stats.quadCount++;
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float textureScale, const glm::vec4& color, int entityID)
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
			s_Data.quadVertexBufferPtr->entityID = entityID;
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

	void Renderer2D::drawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float textureScale, const glm::vec4& color)
	{
		drawQuadRotated({ position.x, position.y, 0.0f }, size, rotation, texture, textureScale, color);
	}

	void Renderer2D::drawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float textureScale, const glm::vec4& color)
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

	void Renderer2D::drawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		drawLine(p0, p1, color, entityID);
		drawLine(p1, p2, color, entityID);
		drawLine(p2, p3, color, entityID);
		drawLine(p3, p0, color, entityID);
	}

	void Renderer2D::drawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 linePositions[4];

		for(size_t i = 0; i < 4; i++)
			linePositions[i] = transform * s_Data.quadVertexPositions[i];

		for(int i = 0; i < 4; i++)
			drawLine(linePositions[i], linePositions[(i+1)%4], color, entityID);
	}

	void Renderer2D::drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.texture)
			drawQuad(transform, src.texture, src.textureScale, src.color, entityID);
		else
			drawQuad(transform, src.color, entityID);
	}

	void Renderer2D::drawCircle(const glm::mat4 transform, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		GCE_PROFILE_FUNCTION();

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.circleVertexBufferPtr->worldPosition = transform * s_Data.quadVertexPositions[i];
			s_Data.circleVertexBufferPtr->localPosition = s_Data.quadVertexPositions[i] * 2.0f;
			s_Data.circleVertexBufferPtr->color = color;
			s_Data.circleVertexBufferPtr->fade = fade;
			s_Data.circleVertexBufferPtr->thickness = thickness;
			s_Data.circleVertexBufferPtr->entityID = entityID;
			s_Data.circleVertexBufferPtr++;
		}

		s_Data.circleIndexCount += 6;

		s_Data.stats.quadCount++;
	}

	void Renderer2D::drawLine(const glm::vec3& x, const glm::vec3& y, const glm::vec4& color, int entityID)
	{
		GCE_PROFILE_FUNCTION();

		s_Data.lineVertexBufferPtr->position = x;
		s_Data.lineVertexBufferPtr->color = color;
		s_Data.lineVertexBufferPtr->entityID = entityID;

		s_Data.lineVertexBufferPtr++;

		s_Data.lineVertexBufferPtr->position = y;
		s_Data.lineVertexBufferPtr->color = color;
		s_Data.lineVertexBufferPtr->entityID = entityID;

		s_Data.lineVertexBufferPtr++;

		s_Data.lineVertexCount += 2;
	}

	float Renderer2D::getLineWidth()
	{
		return s_Data.lineWidth;
	}

	void Renderer2D::setLineWidth(float lineWidth)
	{
		s_Data.lineWidth = lineWidth;	
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