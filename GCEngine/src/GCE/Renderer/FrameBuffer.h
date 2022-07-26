#pragma once
#include "GCE/Core/Core.h"

namespace GCE
{
	struct FrameBufferSpecification
	{
		unsigned int width, height;
		unsigned int samples = 1;

		bool swapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(unsigned int width, unsigned int height) = 0;
		
		//virtual FrameBufferSpecification& getSpecification() = 0;
		virtual const FrameBufferSpecification& getSpecification() const = 0;
		virtual unsigned int getColorAttachmentRendererID() const = 0;

	public:
		static Ref<FrameBuffer> create(const FrameBufferSpecification& spec);

	private:

	};
}