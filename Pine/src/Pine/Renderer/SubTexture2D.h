#pragma once

#include "Pine/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace Pine
{
	class SubTexture2D
	{
	public:

		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D>& GetTexture() const
		{
			return m_Texture;
		}

		const glm::vec2* GetTextureCoordinates() const
		{
			return m_TextureCoordinates;
		}

		static Ref<SubTexture2D> CreateFromCoordinates(const Ref<Texture2D>& texture, const glm::vec2& coordinates, const glm::vec2& cellSize, const glm::vec2& spriteSize = glm::vec2(1.0f, 1.0f));

	private:

		Ref<Texture2D> m_Texture;

		glm::vec2 m_TextureCoordinates[4];

	};
}