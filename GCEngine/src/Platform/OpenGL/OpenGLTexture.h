#pragma once
#include "GCE/Renderer/Texture.h"

#include <glad/glad.h>

namespace GCE
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(unsigned int width, unsigned int height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual unsigned int getWidth() const override { return m_Width; }
		virtual unsigned int getHeight() const override { return m_Height; }

		virtual unsigned int getRendererID() const override { return m_RendererID; }

		virtual void setData(void* data, unsigned int size) override;
		
		virtual void bind(unsigned int slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}

		const std::string& getPath() const { return m_Path; }

	private:
		std::string m_Path;
		unsigned int m_Width, m_Height;
		unsigned int m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}