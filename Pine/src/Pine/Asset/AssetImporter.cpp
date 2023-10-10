#include "pnpch.h"
#include "AssetImporter.h"

#include "Pine/Asset/TextureImporter.h"
#include "Pine/Asset/SceneImporter.h"

#include <map>

namespace Pine
{
	using AssetImportFunction = std::function<Ref<Asset>(AssetHandle, const AssetMetadata&)>;
	static std::map<AssetType, AssetImportFunction> s_AssetImportFunctions = {
		{ AssetType::Texture2D, TextureImporter::ImportTexture2D },
		{ AssetType::Scene, SceneImporter::ImportScene }
	};

	Ref<Asset> AssetImporter::ImportAsset(AssetHandle handle, const AssetMetadata& metadata)
	{
		if (s_AssetImportFunctions.find(metadata.Type) == s_AssetImportFunctions.end()) 
		{
			PN_CORE_ERROR("No importer available for asset type: {}", static_cast<uint16_t>(metadata.Type));
			return nullptr;
		}

		return s_AssetImportFunctions.at(metadata.Type)(handle, metadata);
	}
}
