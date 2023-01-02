#include "pnpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Pine
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			PN_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();
		}

		PN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}