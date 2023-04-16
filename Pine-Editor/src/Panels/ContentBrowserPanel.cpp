#include "ContentBrowserPanel.h"

#include "Pine/Project/Project.h"

#include <imgui/imgui.h>
#include <IconsForkAwesome.h>

namespace Pine
{
	namespace Utils
	{
		std::vector<std::string> SplitString(const std::string& source, const std::string& delimiter)
		{
			std::size_t start = 0, end = 0, delimiterLength = delimiter.size();
			std::string token;
			std::vector<std::string> tokens;
			while ((end = source.find(delimiter, start)) != std::string::npos) {
				token = source.substr(start, end - start);
				start = end + delimiterLength;
				tokens.push_back(token);
			}

			tokens.push_back(source.substr(start));
			return tokens;
		}
	}

	ContentBrowserPanel::ContentBrowserPanel()
		: m_BaseDirectory(Project::GetAssetDirectory()), m_CurrentDirectory(m_BaseDirectory)
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

	void ContentBrowserPanel::GoToFolder(const std::vector<std::string>& directories, std::size_t index)
	{
		std::string relativePath;
		for (std::size_t i = 0; i < index; i++)
		{
			const auto& directory = directories[i];
			relativePath += "\\" + directory;
		}

		m_CurrentDirectory = m_BaseDirectory.string() + relativePath;

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
			auto currentRelativePath = std::filesystem::relative(m_CurrentDirectory, m_BaseDirectory);
			std::vector<std::string> directories = Utils::SplitString(currentRelativePath.string(), "\\");

			if (ImGui::Button(ICON_FK_FOLDER " Content", ImVec2(0.0f, 28.0f)))
			{
				GoToFolder(directories, 0);
			}

			if (m_CurrentDirectory != m_BaseDirectory)
			{
				for (std::size_t i = 0; i < directories.size(); i++)
				{
					const auto& directory = directories[i];

					ImGui::SameLine(0.0f, 8.0f);
					ImGui::Button(">", ImVec2(0.0f, 28.0f));
					ImGui::SameLine(0.0f, 8.0f);

					if (ImGui::Button(directory.c_str(), ImVec2(0.0f, 28.0f)))
						GoToFolder(directories, i + 1);
				}
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
				std::filesystem::path relativePath(path);
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
