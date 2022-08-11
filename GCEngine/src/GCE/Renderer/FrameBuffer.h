#pragma once
#include "GCE/Core/Core.h"

namespace GCE
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		//Color
		RGBA8,
		RED_INTEGER,

		//Depth/stencil
		DEPTH24STENCIL8,

		//Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format) : textureFormat(format) {}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments) : attachments(attachments) {}


		std::vector<FramebufferTextureSpecification> attachments;
	};

	struct FrameBufferSpecification
	{
		unsigned int width, height;
		unsigned int samples = 1;
		FramebufferAttachmentSpecification attachmentSpecification;

		bool swapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(unsigned int width, unsigned int height) = 0;
		virtual int readPixel(unsigned int attachmentIndex, int x, int y) = 0;
		
		//virtual FrameBufferSpecification& getSpecification() = 0;
		virtual const FrameBufferSpecification& getSpecification() const = 0;
		virtual unsigned int getColorAttachmentRendererID(unsigned int index = 0) const = 0;

	public:
		static Ref<FrameBuffer> create(const FrameBufferSpecification& spec);

	private:

	};
}