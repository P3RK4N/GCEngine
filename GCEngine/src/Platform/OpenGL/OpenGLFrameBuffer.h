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
		virtual unsigned int getColorAttachmentRendererID(unsigned int index = 0) const { GCE_CORE_ASSERT(index >= 0 && index < m_ColorAttachments.size(), "Invalid index"); return m_ColorAttachments[index]; }

		virtual void clearColorAttachment(unsigned index, int value) const override;

		virtual void resize(unsigned int width, unsigned int height) override;
		virtual int readPixel(unsigned attachmentIndex, int x, int y) override;

		void Invalidate();

	private:
		FrameBufferSpecification m_Specification;
		unsigned int m_RendererID = 0;
		//unsigned int m_ColorAttachment = 0;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<unsigned int> m_ColorAttachments;
		unsigned int m_DepthAttachment = 0;
	};
}