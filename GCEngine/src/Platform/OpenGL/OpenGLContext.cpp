#include "GCEPCH.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace GCE
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		GCE_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::init()
	{
		GCE_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GCE_CORE_ASSERT(status, "Glad failed to load!");

		GCE_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		GCE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		GCE_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::swapBuffers()
	{
		GCE_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}