#pragma once
#include "GCE/Renderer/Texture.h"

namespace GCE
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual unsigned int getWidth() const override { return m_Width; }
		virtual unsigned int getHeight() const override { return m_Height; }
		
		virtual void bind(unsigned int slot = 0) const override;

	private:
		std::string m_Path;
		unsigned int m_Width, m_Height;
		unsigned int m_RendererID;
	};
}