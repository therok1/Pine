#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

namespace Pine
{
	extern const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("res/icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("res/icons/ContentBrowser/FileIcon.png");
		m_TXT_FileIcon = Texture2D::Create("res/icons/ContentBrowser/TXT_FileIcon.png");
		m_PINE_FileIcon = Texture2D::Create("res/icons/ContentBrowser/PINE_FileIcon.png");

		m_FileExtensionIcons.emplace(".txt", m_TXT_FileIcon);
		m_FileExtensionIcons.emplace(".pine", m_PINE_FileIcon);
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		// TODO: Divide into 2 panels; left, with the directory view, and right, with all the files within specified directory.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30.0f, 30.0f));
		ImGui::Begin("Content Browser");
		ImGui::PopStyleVar();
		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
			if (ImGui::Button("<"))
				m_CurrentDirectory = m_CurrentDirectory.parent_path();

		static float thumbnailSize = 128.0f;
		static float padding = 16.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		float columnCount = static_cast<int>(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();
			
			ImGui::PushID(filenameString.c_str());
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : GetFileExtensionIcon(path.extension().string());
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f));
			ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->GetRendererID()), ImVec2(thumbnailSize, thumbnailSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
			ImGui::PopStyleColor();

			if (ImGui::BeginDragDropSource())
			{
				auto relativePath = std::filesystem::relative(path, g_AssetPath);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
				{
					m_CurrentDirectory /= path.filename();
				}
			}

			ImGui::TextWrapped(filenameString.c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::End();
	}

	Ref<Texture2D> ContentBrowserPanel::GetFileExtensionIcon(const std::string& key) const
	{
		if (m_FileExtensionIcons.find(key) != m_FileExtensionIcons.end())
			return m_FileExtensionIcons.at(key);
		else
			return m_FileIcon;
	}
}
