#include "SceneHierarchyPanel.h"

#include "Pine/Scene/Components.h"
#include "Pine/Scripting/ScriptEngine.h"
#include "Pine/UI/UI.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <IconsForkAwesome.h>

#include <glm/gtc/type_ptr.hpp>

namespace Pine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (m_Context)
		{
			m_Context->m_Registry.each(
				[&](auto entityID)
				{
					Entity entity(entityID, m_Context.get()); DrawEntityNode(entity);
				}
			);

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};
		}

		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();
		
		ImGui::Begin("Properties");
		
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		
		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	template<typename T>
	inline void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName)
	{
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uint64_t>(static_cast<uint32_t>(entity))), flags, tag.c_str());
		
		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
			ImGui::TreePop();

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f, bool buttons = false)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, buttons ? ImGui::CalcItemWidth() : ImGui::GetContentRegionAvail().x);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 0.0f));

		constexpr float lineThickness = 3.0f;

		constexpr ImVec4 textBase(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
		constexpr ImVec4 textLabel(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 255.0f / 255.0f);

		constexpr ImVec4 redBase(203.0f / 255.0f, 38.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
		constexpr ImVec4 redHovered(255.0f / 255.0f, 47.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
		constexpr ImVec4 redActive(203.0f / 255.0f, 38.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);

		constexpr ImVec4 greenBase(103.0f / 255.0f, 169.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
		constexpr ImVec4 greenHovered(126.0f / 255.0f, 207.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
		constexpr ImVec4 greenActive(103.0f / 255.0f, 169.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);

		constexpr ImVec4 blueBase(44.0f / 255.0f, 126.0f / 255.0f, 237.0f / 255.0f, 255.0f / 255.0f);
		constexpr ImVec4 blueHovered(90.0f / 255.0f, 152.0f / 255.0f, 237.0f / 255.0f, 255.0f / 255.0f);
		constexpr ImVec4 blueActive(44.0f / 255.0f, 126.0f / 255.0f, 237.0f / 255.0f, 255.0f / 255.0f);


		float lineHeight = GImGui->Font->FontSize * ImGui::GetIO().FontGlobalScale + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize(lineHeight, lineHeight);

		if (buttons)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, textBase);
			ImGui::PushStyleColor(ImGuiCol_Button, redBase);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, redHovered);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, redActive);
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(4);

			ImGui::SameLine();
		}

		ImGui::PushStyleColor(ImGuiCol_Text, textBase);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, ImGui::GetStyle().FramePadding.y));
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		if (!buttons)
		{
			ImVec2 buttonPos = ImGui::GetItemRectMin();
			ImVec2 buttonDimensions = ImGui::GetItemRectSize();

			ImVec2 lineStart(buttonPos.x + 4, buttonPos.y + 2);
			ImVec2 lineEnd(buttonPos.x + 4, buttonPos.y + buttonDimensions.y - 4);

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddLine(lineStart, lineEnd, ImGui::GetColorU32(redBase), lineThickness);
		}

		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (buttons)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, textBase);
			ImGui::PushStyleColor(ImGuiCol_Button, greenBase);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, greenHovered);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, greenActive);
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(4);
			ImGui::SameLine();
		}

		ImGui::PushStyleColor(ImGuiCol_Text, textBase);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, ImGui::GetStyle().FramePadding.y));
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		
		if (!buttons)
		{
			ImVec2 buttonPos = ImGui::GetItemRectMin();
			ImVec2 buttonDimensions = ImGui::GetItemRectSize();

			ImVec2 lineStart(buttonPos.x + 4, buttonPos.y + 2);
			ImVec2 lineEnd(buttonPos.x + 4, buttonPos.y + buttonDimensions.y - 4);

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddLine(lineStart, lineEnd, ImGui::GetColorU32(greenBase), lineThickness);
		}

		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (buttons)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, textBase);
			ImGui::PushStyleColor(ImGuiCol_Button, blueBase);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, blueHovered);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, blueActive);
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(4);
			ImGui::SameLine();
		}

		ImGui::PushStyleColor(ImGuiCol_Text, textBase);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, GImGui->Style.FramePadding.y));
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		
		if (!buttons)
		{
			ImVec2 buttonPos = ImGui::GetItemRectMin();
			ImVec2 buttonDimensions = ImGui::GetItemRectSize();

			ImVec2 lineStart(buttonPos.x + 4, buttonPos.y + 2);
			ImVec2 lineEnd(buttonPos.x + 4, buttonPos.y + buttonDimensions.y - 4);

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddLine(lineStart, lineEnd, ImGui::GetColorU32(blueBase), lineThickness);
		}

		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& label, Entity entity, UIFunction function)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed 
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			ImGui::Dummy(ImVec2(0.0f, 4.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
			ImGui::PushFont(boldFont);

			float lineHeight = GImGui->Font->FontSize * ImGui::GetIO().FontGlobalScale + GImGui->Style.FramePadding.y * 2.0f;
			bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(T).hash_code()), treeNodeFlags, label.c_str());

			ImGui::PopFont();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button(ICON_FK_COG, ImVec2(lineHeight, lineHeight)))
				ImGui::OpenPopup("ComponentSettings");

			ImGui::PopStyleVar();

			if (opened)
				ImGui::Dummy(ImVec2(0.0f, 4.0f));

			bool componentRemoved = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					componentRemoved = true;

				ImGui::EndPopup();
			}

			if (opened)
			{
				function(component);
				ImGui::TreePop();
			}

			if (componentRemoved)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 75.0f - GImGui->Style.ItemSpacing.x);
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
				tag = std::string(buffer);
			ImGui::PopItemWidth();
		}

		ImGui::SameLine();

		float lineHeight = GImGui->Font->FontSize * ImGui::GetIO().FontGlobalScale + GImGui->Style.FramePadding.y * 2.0f;
		
		if (ImGui::Button(ICON_FK_PLUS " Add", ImVec2(75.0f, lineHeight)))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<CircleRendererComponent>("Circle Renderer");
			DisplayAddComponentEntry<ScriptComponent>("Script");
			DisplayAddComponentEntry<RigidBody2DComponent>("Rigid Body 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D");

			ImGui::EndPopup();
		}

		DrawComponent<TransformComponent>("Transform", entity, 
			[](auto& component)
			{
				DrawVec3Control("Translation", component.Translation);
				ImGui::Spacing();
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				ImGui::Spacing();
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
				ImGui::Spacing();
			}
		);

		DrawComponent<CameraComponent>("Camera", entity, 
			[](auto& component)
			{
				auto& camera = component.Camera;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[static_cast<std::size_t>(camera.GetProjectionType())];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (std::size_t i = 0; i < 2; i++)
					{
						const bool selected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], selected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
						}

						if (selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &verticalFOV))
						camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV));

					float nearClip = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &nearClip))
						camera.SetPerspectiveNearClip(nearClip);

					float farClip = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &farClip))
						camera.SetPerspectiveFarClip(farClip);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float nearClip = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &nearClip))
						camera.SetOrthographicNearClip(nearClip);

					float farClip = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &farClip))
						camera.SetOrthographicFarClip(farClip);
				}

				ImGui::Checkbox("Primary", &component.Primary);
				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
			}
		);

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, 
			[](auto& component) 
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

				ImGui::Button("Texture", ImVec2(100.0f, 100.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = static_cast<const wchar_t*>(payload->Data);
						std::filesystem::path texturePath(path);
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture->IsLoaded())
							component.Texture = texture;
						else
							PN_CORE_WARN("Could not load texture '{0}'!", texturePath.filename().string());
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
			}
		);

		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, 
			[](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
			}
		);

		DrawComponent<ScriptComponent>("Script", entity, 
			[entity, scene = m_Context](auto& component) mutable
			{
				bool scriptClassExists = ScriptEngine::EntityClassExists(component.ClassName);

				static char buffer[64];
				strcpy_s(buffer, sizeof(buffer), component.ClassName.c_str());

				UI::ScopedStyleColor textColor(ImGuiCol_Text, ImVec4(229.0f / 255.0f, 106.0f / 255.0f, 98.0f / 255.0f, 255.0f / 255.0f), !scriptClassExists);

				if (ImGui::InputText("Class", buffer, sizeof(buffer)))
				{
					component.ClassName = buffer;
					return;
				}

				bool sceneRunning = scene->IsRunning();
				if (sceneRunning)
				{
					Ref<ScriptInstance> scriptInstance = ScriptEngine::GetEntityScriptInstance(entity.GetUUID());
					if (scriptInstance)
					{
						const auto& fields = scriptInstance->GetScriptClass()->GetFields();
						for (const auto& [name, field] : fields)
						{
							if (field.Type == ScriptFieldType::Float)
							{
								float data = scriptInstance->GetFieldValue<float>(name);
								if (ImGui::DragFloat(name.c_str(), &data))
								{
									scriptInstance->SetFieldValue(name, data);
								}
							}
						}
					}
				}
				else
				{
					if (scriptClassExists)
					{
						Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(component.ClassName);
						const auto& fields = entityClass->GetFields();

						auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
						for (const auto& [name, field] : fields)
						{
							if (entityFields.find(name) != entityFields.end())
							{
								ScriptFieldInstance& scriptField = entityFields.at(name);

								if (field.Type == ScriptFieldType::Float)
								{
									float data = scriptField.GetValue<float>();
									if (ImGui::DragFloat(name.c_str(), &data))
										scriptField.SetValue(data);
								}
							}
							else
							{
								if (field.Type == ScriptFieldType::Float)
								{
									float data = 0.0f;
									if (ImGui::DragFloat(name.c_str(), &data))
									{
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
								}
							}
						}
					}
				}
			});

		DrawComponent<RigidBody2DComponent>("Rigid Body 2D", entity,
			[](auto& component)
			{
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeString = bodyTypeStrings[static_cast<std::size_t>(component.Type)];
				if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							component.Type = static_cast<RigidBody2DComponent::BodyType>(i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
			}
		);

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, 
			[](auto& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.01f);
				ImGui::DragFloat2("Size", glm::value_ptr(component.Size), 0.01f);
				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			}
		);

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, 
			[](auto& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.01f);
				ImGui::DragFloat("Radius", &component.Radius, 0.01f);
				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			}
		);
	}
}
