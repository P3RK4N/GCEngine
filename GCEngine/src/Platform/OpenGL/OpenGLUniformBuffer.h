#pragma once
#include "GCE/Renderer/UniformBuffer.h"

namespace GCE
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(unsigned int size, unsigned int binding);
		virtual ~OpenGLUniformBuffer() override;

		virtual void setData(const void* data, unsigned int size, unsigned int offset = 0) override;

	private:
		unsigned int m_RendererID = 0;
	};
}