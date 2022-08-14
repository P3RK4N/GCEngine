#pragma once

#include "GCE/Core/Core.h"

//namespace GCE
//{
//	class UniformBuffer
//	{
//	public:
//		virtual ~UniformBuffer() {}
//		virtual void setData(const void* data, unsigned int size, unsigned int offset = 0) = 0;
//
//		static Ref<UniformBuffer> create(unsigned int size, unsigned int binding);
//	};
//}

namespace GCE {

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}
		virtual void setData(const void* data, unsigned int size, unsigned int offset = 0) = 0;
		
		static Ref<UniformBuffer> create(unsigned int size, unsigned int binding);
	};

}