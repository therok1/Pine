#include "ContentBrowserPanel.h"

#include "Pine/Project/Project.h"
#include "Pine/Asset/TextureImporter.h"

#include <imgui/imgui.h>
#include <IconsForkAwesome.h>

namespace Pine
{
	namespace Utils
	{
		std::vector<std::string> SplitString(const std::string& source, const std::string& delimiter) // TODO: Move string manipulation functions into Utils.h
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
		m_TreeNodes.push_back(TreeNode(".", 0));

		m_DirectoryIcon = TextureImporter::LoadTexture2D("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = TextureImporter::LoadTexture2D("Resources/Icons/ContentBrowser/FileIcon.png");

		RefreshAssetTree();

		m_Mode = Mode::FileSystem;
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
					{

					}
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

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
		ImGui::Begin("Content Browser");
		ImGui::PopStyleVar();

		DrawToolbar();

		const char* label = m_Mode == Mode::Asset ? "Asset" : "File";
		if (ImGui::Button(label))
		{
			m_Mode = m_Mode == Mode::Asset ? Mode::FileSystem : Mode::Asset;
		}

		if (m_CurrentDirectory != std::filesystem::path(m_BaseDirectory))
		{
			ImGui::SameLine();
			if (ImGui::Button("<"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		float columnCount = static_cast<int>(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		if (m_Mode == Mode::Asset)
		{
			TreeNode* node = &m_TreeNodes[0];

			auto currentDir = std::filesystem::relative(m_CurrentDirectory, Project::GetAssetDirectory());
			for (const auto& p : currentDir)
			{
				if (node->Path == currentDir)
					break;

				if (node->Children.find(p) != node->Children.end())
				{
					node = &m_TreeNodes[node->Children[p]];
					continue;
				}
				else
				{
					PN_CORE_ASSERT(false);
				}
			}

			for (const auto& [item, treeNodeIndex] : node->Children)
			{
				bool isDirectory = std::filesystem::is_directory(Project::GetAssetDirectory() / item);

				std::string itemStr = item.generic_string();

				ImGui::PushID(itemStr.c_str());
				Ref<Texture2D> icon = isDirectory ? m_DirectoryIcon : m_FileIcon;
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->GetRendererID()), ImVec2(thumbnailSize, thumbnailSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete"))
					{
						PN_CORE_ASSERT(false, "Not implemented");
					}
					ImGui::EndPopup();
				}

				if (ImGui::BeginDragDropSource())
				{
					AssetHandle handle = m_TreeNodes[treeNodeIndex].Handle;
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", &handle, sizeof(AssetHandle));
					ImGui::Text("Moving \"%s\"", item.filename().string().c_str());
					ImGui::EndDragDropSource();
				}

				ImGui::PopStyleColor();
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (isDirectory)
						m_CurrentDirectory /= item.filename();
				}

				ImGui::TextWrapped(itemStr.c_str());

				ImGui::NextColumn();

				ImGui::PopID();
			}
		}
		else
		{
			for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
			{
				const auto& path = directoryEntry.path();
				std::string filenameString = path.filename().string();

				ImGui::PushID(filenameString.c_str());
				Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f));
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->GetRendererID()), ImVec2(thumbnailSize, thumbnailSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Import"))
					{
						auto relativePath = std::filesystem::relative(path, Project::GetAssetDirectory());
						Project::GetActive()->GetEditorAssetManager()->ImportAsset(relativePath);
						RefreshAssetTree();
					}

					ImGui::EndPopup();
				}

				ImGui::PopStyleColor();

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
		}

		ImGui::Columns(1);

		ImGui::End();
	}

	void ContentBrowserPanel::RefreshAssetTree()
	{
		const auto& assetRegistry = Project::GetActive()->GetEditorAssetManager()->GetAssetRegistry();
		for (const auto& [handle, metadata] : assetRegistry)
		{
			uint32_t currentNodeIndex = 0;

			for (const auto& p : metadata.FilePath)
			{
				auto it = m_TreeNodes[currentNodeIndex].Children.find(p.generic_string());
				if (it != m_TreeNodes[currentNodeIndex].Children.end())
				{
					currentNodeIndex = it->second;
				}
				else
				{
					TreeNode newNode(p, handle);
					newNode.Parent = currentNodeIndex;
					m_TreeNodes.push_back(newNode);

					m_TreeNodes[currentNodeIndex].Children[p] = m_TreeNodes.size() - 1;
					currentNodeIndex = m_TreeNodes.size() - 1;
				}

			}
		}
	}
}
