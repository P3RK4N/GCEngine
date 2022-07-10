#pragma once

#include "GCE/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace GCE
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void init() override;
		virtual void swapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}