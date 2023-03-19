#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

#include <IconsForkAwesome.h>

namespace Pine
{
	extern const std::filesystem::path g_AssetPath = "assets";

	namespace Utils
	{
		std::vector<std::string> SplitString(const std::string& text, const std::string& delimiter)
		{
			std::size_t start = 0, end = 0, delimiterLength = delimiter.size();
			std::string token;
			std::vector<std::string> tokens;
			while ((end = text.find(delimiter, start)) != std::string::npos) {
				token = text.substr(start, end - start);
				start = end + delimiterLength;
				tokens.push_back(token);
			}

			tokens.push_back(text.substr(start));
			return tokens;
		}
	}

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
		m_TXT_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/TXT_FileIcon.png");
		m_PINE_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/PINE_FileIcon.png");

		m_FileExtensionIcons.emplace(".txt", m_TXT_FileIcon);
		m_FileExtensionIcons.emplace(".pine", m_PINE_FileIcon);
	}

	void ContentBrowserPanel::Refresh()
	{
	}

	void ContentBrowserPanel::GoToFolder(const std::string& directory)
	{
		auto& currentPath = m_CurrentDirectory.string();
		if (currentPath.find(directory) != std::string::npos)
			m_CurrentDirectory = currentPath.substr(0, currentPath.find(directory) + directory.size());

		Refresh();
	}

	void ContentBrowserPanel::DrawToolbar()
	{
		constexpr float toolbarHeight = 32.0f;
		constexpr float labelHeight = 28.0f;
		ImVec4 buttonColor(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
		ImVec2 size(ImGui::GetContentRegionAvail().x, toolbarHeight);

		ImGui::BeginChild("Toolbar", size);
		ImGui::SetCursorPosY((toolbarHeight / 2.0f) - (labelHeight / 2.0f));

		if (ImGui::Button(ICON_FK_PLUS " Add", ImVec2(75.0f, 28.0f)))
		{

		}

		ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);

		ImGui::SameLine(0.0f, 5.0f);
		if (ImGui::Button(ICON_FK_EXTERNAL_LINK_SQUARE " Import", ImVec2(0.0f, 28.0f)))
		{
			
		}

		ImGui::SameLine(0.0f, 5.0f);
		if (ImGui::Button(ICON_FK_FILE_TEXT  " Save all", ImVec2(0.0f, 28.0f)))
		{
			
		}

		ImGui::SameLine(0.0f, 15.0f);
		if (ImGui::Button(ICON_FK_ARROW_CIRCLE_O_LEFT, ImVec2(28.0f, 28.0f)))
		{
			
		}

		ImGui::SameLine(0.0f, 0.0f);
		if (ImGui::Button(ICON_FK_ARROW_CIRCLE_O_RIGHT, ImVec2(28.0f, 28.0f)))
		{
			
		}

		// Current folder path
		ImGui::SameLine(0.0f, 5.0f);
		{
			std::vector<std::string> directories = Utils::SplitString(m_CurrentDirectory.string(), "\\");

			if (ImGui::Button(ICON_FK_FOLDER " Content", ImVec2(0.0f, 28.0f)))
			{
				
			}

			for (const auto& directory : directories)
			{
				ImGui::SameLine(0.0f, 8.0f);
				ImGui::Button(">", ImVec2(0.0f, 28.0f));
				ImGui::SameLine(0.0f, 8.0f);

				if (ImGui::Button(directory.c_str(), ImVec2(0.0f, 28.0f)))
					GoToFolder(directory);
			}
		}

		ImGui::SameLine(size.x - 90.0f, 0.0f);
		if (ImGui::Button(ICON_FK_COG " Settings", ImVec2(90.0f, 28.0f)))
		{

		}

		ImGui::PopStyleColor();

		ImGui::EndChild();
		ImGui::Dummy(ImVec2(size.x, 2.0f));
	}

	void ContentBrowserPanel::DrawFolderTreeView()
	{
		
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
		ImGui::Begin("Content Browser");
		ImGui::PopStyleVar();

		DrawToolbar();
		DrawFolderTreeView();

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
