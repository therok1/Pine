#pragma once

#include "Pine/Renderer/Texture.h"

#include <filesystem>

namespace Pine
{
	class ContentBrowserPanel
	{
	public:

		ContentBrowserPanel();

	private:

		void Refresh();
		void GoToFolder(const std::vector<std::string>& directories, std::size_t index);

		void DrawToolbar();
		void DrawFolderTreeView();

	public:

		void OnImGuiRender();

	private:

		Ref<Texture2D> GetFileExtensionIcon(const std::string& key) const;

	private:

		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_TXT_FileIcon, m_PINE_FileIcon;

		std::unordered_map<std::string, Ref<Texture2D>> m_FileExtensionIcons;

	};
}
