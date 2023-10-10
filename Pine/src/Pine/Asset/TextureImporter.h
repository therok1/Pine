#pragma once

#include "Pine/Asset/Asset.h"
#include "Pine/Asset/AssetMetadata.h"

#include "Pine/Renderer/Texture.h"

namespace Pine
{

	class TextureImporter
	{
	public:

		static Ref<Texture2D> ImportTexture2D(AssetHandle handle, const AssetMetadata& metadata);
		static Ref<Texture2D> LoadTexture2D(const std::filesystem::path& path);

	};
}
