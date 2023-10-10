#pragma once

#include "Pine/Asset/Asset.h"

#include <filesystem>

namespace Pine
{
	struct AssetMetadata
	{
		AssetType Type = AssetType::None;
		std::filesystem::path FilePath;

		operator bool() const { return Type != AssetType::None; }
	};
}
