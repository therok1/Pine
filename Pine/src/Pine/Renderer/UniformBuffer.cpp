#include "pnpch.h"
#include "UniformBuffer.h"

#include "Pine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Pine
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			PN_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		PN_CORE_ASSERT(false, "Invalid RendererAPI!");
		return nullptr;
	}
}
