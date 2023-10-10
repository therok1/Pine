#pragma once

#include "Pine/Asset/Asset.h"
#include "Pine/Asset/AssetMetadata.h"


#include "Pine/Scene/Scene.h"

namespace Pine
{
	class SceneImporter
	{
	public:

		static Ref<Scene> ImportScene(AssetHandle handle, const AssetMetadata& metadata);
		static Ref<Scene> LoadScene(const std::filesystem::path& path);

		static void SaveScene(Ref<Scene> scene, const std::filesystem::path& path);

	};
}
