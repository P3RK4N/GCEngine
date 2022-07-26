#pragma once
#include "GCE/Renderer/FrameBuffer.h"

namespace GCE
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual void bind() override;
		virtual void unbind() override;

		virtual const FrameBufferSpecification& getSpecification() const override { return m_Specification; }
		virtual unsigned int getColorAttachmentRendererID() const { return m_ColorAttachment; }

		virtual void resize(unsigned int width, unsigned int height) override;
		void Invalidate();

	private:
		FrameBufferSpecification m_Specification;
		unsigned int m_RendererID = 0;
		unsigned int m_ColorAttachment = 0;
		unsigned int m_DepthAttachment = 0;
	};
}