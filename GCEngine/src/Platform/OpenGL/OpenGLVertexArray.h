#pragma once

#include "GCE/Renderer/VertexArray.h"

namespace GCE
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		virtual ~OpenGLVertexArray();
		OpenGLVertexArray();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& getIndexBuffer() const { return m_IndexBuffer; }

	private:
		unsigned int m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}