#include "pnpch.h"
#include "Framebuffer.h"

#include "Pine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Pine
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			PN_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(spec);
		}

		PN_CORE_ASSERT(false, "Invalid RendererAPI!");
		return nullptr;
	}
}