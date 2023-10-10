#pragma once

#include "Pine/Asset/AssetMetadata.h"

namespace Pine
{
	class AssetImporter
	{
	public:
		
		static Ref<Asset> ImportAsset(AssetHandle handle, const AssetMetadata& metadata);

	};
}
