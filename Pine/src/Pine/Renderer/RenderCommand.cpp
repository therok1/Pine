#include "pnpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Pine
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}