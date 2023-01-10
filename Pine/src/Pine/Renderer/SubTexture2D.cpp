#include "pnpch.h"
#include "SubTexture2D.h"

namespace Pine
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TextureCoordinates[0] = glm::vec2(min.x, min.y);
		m_TextureCoordinates[1] = glm::vec2(max.x, min.y);
		m_TextureCoordinates[2] = glm::vec2(max.x, max.y);
		m_TextureCoordinates[3] = glm::vec2(min.x, max.y);
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoordinates(const Ref<Texture2D>& texture, const glm::vec2& coordinates, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{
		glm::vec2 min = glm::vec2((coordinates.x * cellSize.x) / texture->GetWidth(), (coordinates.y * cellSize.y) / texture->GetHeight());
		glm::vec2 max = glm::vec2(((coordinates.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((coordinates.y + spriteSize.y) * cellSize.y) / texture->GetHeight());
		return CreateRef<SubTexture2D>(texture, min, max);
	}
}