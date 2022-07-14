#include "GCEPCH.h"
#include "GCE/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace GCE
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}

