#pragma once

#include "Pine/Renderer/Texture.h"

#include <filesystem>

namespace Pine
{
	class ContentBrowserPanel
	{
	public:

		ContentBrowserPanel();

		void OnImGuiRender();

	private:

		Ref<Texture2D> GetFileExtensionIcon(const std::string& key) const;

	private:

		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;

		std::unordered_map<std::string, Ref<Texture2D>> m_FileExtensionIcons;

	};
}
