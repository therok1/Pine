#pragma once

#include "Pine/Core/UUID.h"

namespace Pine 
{

	using AssetHandle = UUID;

	enum class AssetType : uint16_t
	{
		None = 0,
		Scene,
		Texture2D
	};

	std::string_view AssetTypeToString(AssetType type);
	AssetType AssetTypeFromString(std::string_view assetType);

	class Asset
	{
	public:
		
		AssetHandle Handle;

		virtual AssetType GetType() const = 0;

	};

}
