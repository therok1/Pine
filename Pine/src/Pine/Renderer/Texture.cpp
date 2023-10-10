#include "pnpch.h"
#include "Texture.h"

#include "Pine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Pine
{
	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification, Buffer data)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			PN_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(specification, data);
		}

		PN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
