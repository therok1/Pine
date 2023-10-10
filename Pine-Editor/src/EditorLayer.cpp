#include "EditorLayer.h"

#include "Pine/Scene/SceneSerializer.h"
#include "Pine/Utils/PlatformUtils.h"
#include "Pine/Math/Math.h"
#include "Pine/Scripting/ScriptEngine.h"
#include "Pine/Renderer/Font.h"

#include "Pine/Asset/AssetManager.h"
#include "Pine/Asset/TextureImporter.h"
#include "Pine/Asset/SceneImporter.h"

#include <imgui/imgui.h>
#include <ImGuizmo.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Pine
{
	static Ref<Font> s_Font;

	EditorLayer::EditorLayer()
		:
		Layer("EditorLayer"),
		m_CameraController(1280.0f / 720.0f)
	{
		s_Font = Font::GetDefault();
	}

	void EditorLayer::OnAttach()
	{
		PN_PROFILE_FUNCTION();

		m_PlayIcon = TextureImporter::LoadTexture2D("Resources/Icons/PlayIcon.png");
		m_PauseIcon = TextureImporter::LoadTexture2D("Resources/Icons/PauseIcon.png");
		m_ResumeIcon = TextureImporter::LoadTexture2D("Resources/Icons/ResumeIcon.png");
		m_StepIcon = TextureImporter::LoadTexture2D("Resources/Icons/StepIcon.png");
		m_StopIcon = TextureImporter::LoadTexture2D("Resources/Icons/StopIcon.png");
		m_SimulateIcon = TextureImporter::LoadTexture2D("Resources/Icons/SimulateIcon.png");

		FramebufferSpecification spec;
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);

		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;

		auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		if (commandLineArgs.Count > 1)
		{
			auto projectFilePath = commandLineArgs[1];
			OpenProject(projectFilePath);
		}
		else
		{
			if (!OpenProject())
				Application::Get().Close();
		}

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		Renderer2D::SetLineWidth(4.0f);
	}

	void EditorLayer::OnDetach()
	{
		PN_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		PN_PROFILE_FUNCTION();

		m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		m_Framebuffer->ClearAttachment(1, -1);

		switch (m_SceneState)
		{
		case SceneState::Edit:
			if (m_ViewportFocused)
				m_CameraController.OnUpdate(ts);

			m_EditorCamera.OnUpdate(ts);

			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			break;
		case SceneState::Play:
			m_ActiveScene->OnUpdateRuntime(ts);
			break;
		case SceneState::Simulate:
			m_EditorCamera.OnUpdate(ts);

			m_ActiveScene->OnUpdateSimulation(ts, m_EditorCamera);
			break;
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		my = viewportSize.y - my;

		int mouseX = static_cast<int>(mx);
		int mouseY = static_cast<int>(my);

		if (mouseX >= 0 && mouseY >= 0 && mouseX < static_cast<int>(viewportSize.x) && mouseY < static_cast<int>(viewportSize.y))
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity(static_cast<entt::entity>(pixelData), m_ActiveScene.get());
		}

		OnOverlayRender();

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		PN_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
				| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWindowSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 400.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWindowSize;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
					OpenProject();

				ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(36.0f / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 255.0f / 255.0f));
				ImGui::Separator();
				ImGui::PopStyleColor();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(36.0f / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 255.0f / 255.0f));
				ImGui::Separator();
				ImGui::PopStyleColor();

				if (ImGui::MenuItem("Exit", "Alt+F4"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Script"))
			{
				if (ImGui::MenuItem("Reload assembly", "Ctrl+R"))
					ScriptEngine::ReloadAssembly();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel->OnImGuiRender();

		ImGui::Begin("Render Stats");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::Begin("Debug");
		ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);
		ImGui::Text("ImGui ActiveID: %u", Application::Get().GetImGuiLayer()->GetActiveWidgetID());
		ImGui::Text("ImGui FPS: %.02f", 1.0f / io.DeltaTime);
		ImGui::Image(reinterpret_cast<ImTextureID>(s_Font->GetAtlasTexture()->GetRendererID()), ImVec2(512, 512), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		ImVec2 viewportMinRegion = ImGui::GetWindowContentRegionMin();
		ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		ImVec2 viewportOffset = ImGui::GetWindowPos();

		m_ViewportBounds[0] = glm::vec2(viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y);
		m_ViewportBounds[1] = glm::vec2(viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y);

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);

		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<ImTextureID>(textureID), ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				AssetHandle handle = *static_cast<AssetHandle*>(payload->Data);
				OpenScene(handle);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1 && !Input::IsKeyPressed(Key::LeftAlt))
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Runtime Camera

			//auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			//const glm::mat4& cameraProjection = camera.GetProjection();
			//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor Camera

			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = transformComponent.GetTransform();

			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;

			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(
				glm::value_ptr(cameraView),
				glm::value_ptr(cameraProjection),
				static_cast<ImGuizmo::OPERATION>(m_GizmoType),
				ImGuizmo::LOCAL,
				glm::value_ptr(transform),
				nullptr,
				snap ? snapValues : nullptr
			);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - transformComponent.Rotation;
				transformComponent.Translation = translation;
				transformComponent.Rotation += deltaRotation;
				transformComponent.Scale = scale;
			}
		}

		UI_Toolbar();

		ImGui::End();
		ImGui::PopStyleVar();


		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_CameraController.OnEvent(event);

		if (m_SceneState == SceneState::Edit)
			m_EditorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(PN_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(PN_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<WindowDropEvent>(PN_BIND_EVENT_FN(EditorLayer::OnWindowDrop));
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 corner = ImGui::GetCursorPos();
		ImVec2 size = ImGui::GetContentRegionAvail();

		constexpr float buttonSize = 32.0f;
		constexpr float padding = 8.0f;

		ImVec4 containerColor(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 63.75f / 255.0f);

		ImVec4 buttonColor = style.Colors[ImGuiCol_Button];
		buttonColor.w = 0.0f / 255.0f;

		static int numButtons = 2;
		float toolbarWidth = (buttonSize + style.FramePadding.x * 2) * numButtons + padding * (numButtons + 1);
		
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2((ImGui::GetWindowContentRegionMax().x * 0.5f) - (toolbarWidth * 0.5f), ImGui::GetWindowContentRegionMin().y + 2 * padding));

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, containerColor);
		ImGui::BeginChild("##SceneButtons", ImVec2(toolbarWidth, buttonSize + 2 * style.FramePadding.y + 2 * padding));
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::SameLine(padding, 0.0f);

		ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonColor);

		bool hasPlayButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play;
		bool hasSimulateButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate;
		bool hasPauseButton = m_SceneState != SceneState::Edit;

		if (m_ActiveScene->IsPaused())
			numButtons = 3;
		else
			numButtons = 2;

		// Play Button

		if (hasPlayButton)
		{
			ImGui::SetCursorPosY(padding);
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_PlayIcon : m_StopIcon;
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(icon->GetRendererID())), ImVec2(buttonSize, buttonSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f)))
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
					OnScenePlay();
				else if (m_SceneState == SceneState::Play)
					OnSceneStop();
			}
		}

		// Simulate Button

		if (hasSimulateButton)
		{
			if (hasPlayButton)
				ImGui::SameLine(0.0f, padding);

			ImGui::SetCursorPosY(padding);
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_SimulateIcon : m_StopIcon;
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(icon->GetRendererID())), ImVec2(buttonSize, buttonSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f)))
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
					OnSceneSimulate();
				else if (m_SceneState == SceneState::Simulate)
					OnSceneStop();
			}
		}


		// Step Button

		if (hasPauseButton)
		{
			bool isPaused = m_ActiveScene->IsPaused();
			ImGui::SameLine(0.0f, padding);
			ImGui::SetCursorPosY(padding);
			Ref<Texture2D> icon = !isPaused ? m_PauseIcon : m_ResumeIcon;
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(icon->GetRendererID())), ImVec2(buttonSize, buttonSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f)))
			{
				m_ActiveScene->SetPaused(!isPaused);
			}

			if (isPaused)
			{
				ImGui::SameLine(0.0f, padding);
				ImGui::SetCursorPosY(padding);
				Ref<Texture2D> icon = m_StepIcon;
				bool isPaused = m_ActiveScene->IsPaused();
				if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(icon->GetRendererID())), ImVec2(buttonSize, buttonSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f)))
				{
					m_ActiveScene->Step(50);
				}
			}
		}

		ImGui::PopStyleColor(3);

		ImGui::EndChild();
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.IsRepeat())
			return false;

		bool ctrl = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (event.GetKeyCode())
		{
		case Key::N:
			if (ctrl)
				NewScene();
			break;
		case Key::O:
			if (ctrl)
				OpenProject();
			break;
		case Key::S:
			if (ctrl)
				if (shift)
					SaveSceneAs();
				else
					SaveScene();
			break;
		case Key::D:
			if (ctrl)
				OnDuplicateEntity();
			break;
		case Key::Q:
			if (!ImGuizmo::IsUsing())
				m_GizmoType = -1;
			break;
		case Key::W:
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case Key::E:
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case Key::R:
			if (ctrl)
				ScriptEngine::ReloadAssembly();
			else
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		case Key::Delete:
			if (Application::Get().GetImGuiLayer()->GetActiveWidgetID() == 0)
			{
				Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
				if (selectedEntity)
				{
					m_SceneHierarchyPanel.SetSelectedEntity({});
					m_ActiveScene->DestroyEntity(selectedEntity);
				}
				break;
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		if (event.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (m_SceneHierarchyPanel.GetSelectedEntity())
			{
				if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
					m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
			}
			else
			{
				if (m_ViewportHovered && !Input::IsKeyPressed(Key::LeftAlt))
					m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
			}
		}
		return false;
	}

	bool EditorLayer::OnWindowDrop(WindowDropEvent& event)
	{
		return true;
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
			if (!camera)
				return;

			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [transformComponent, boxCollider2DComponent] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					glm::vec3 translation = transformComponent.Translation + glm::vec3(boxCollider2DComponent.Offset, 0.001f);
					glm::vec3 scale = transformComponent.Scale * glm::vec3(boxCollider2DComponent.Size * 2.0f, 1.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), transformComponent.Translation)
						* glm::rotate(glm::mat4(1.0f), transformComponent.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::translate(glm::mat4(1.0f), glm::vec3(boxCollider2DComponent.Offset, 0.001f))
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawRect(transform, glm::vec4(0.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f));
				}
			}

			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [transformComponent, circleCollider2DComponent] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					glm::vec3 translation = transformComponent.Translation + glm::vec3(circleCollider2DComponent.Offset, 0.001f);
					glm::vec3 scale = transformComponent.Scale * glm::vec3(circleCollider2DComponent.Radius * 2.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawCircle(transform, glm::vec4(0.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f), 0.03f);
				}
			}
		}

		if (Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity())
		{
			TransformComponent transformComponent = selectedEntity.GetComponent<TransformComponent>();
			Renderer2D::DrawRect(transformComponent.GetTransform(), glm::vec4(82.0f / 255.0f, 171.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));
		}

		Renderer2D::EndScene();
	}

	void EditorLayer::NewProject()
	{
		Project::New();
	}

	bool EditorLayer::OpenProject()
	{
		std::string filepath = FileDialogs::OpenFile("Pine Project (*.pproj)\0*.pproj\0");
		if (filepath.empty())
			return false;
		
		OpenProject(filepath);
		return true;
	}

	void EditorLayer::OpenProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			ScriptEngine::Init();

			AssetHandle startScene = Project::GetActive()->GetConfig().StartScene;
			std::cout << startScene << std::endl;
			if (startScene)
				OpenScene(startScene);
			m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>();
		}
	}

	void EditorLayer::SaveProject()
	{
		// Project::SaveActive();
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		/*std::string filepath = FileDialogs::OpenFile("Pine Scene (*.pine)\0*.pine\0");
		if (!filepath.empty())
			OpenScene(filepath);*/
	}

	void EditorLayer::OpenScene(AssetHandle handle)
	{
		PN_CORE_ASSERT(handle);

		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		Ref<Scene> readOnlyScene = AssetManager::GetAsset<Scene>(handle);
		Ref<Scene> newScene = Scene::Copy(readOnlyScene);

		m_EditorScene = newScene;
		m_SceneHierarchyPanel.SetContext(m_EditorScene);

		m_ActiveScene = m_EditorScene;
		m_EditorScenePath = Project::GetActive()->GetEditorAssetManager()->GetFilePath(handle);
	}

	void EditorLayer::SaveScene()
	{
		if (!m_EditorScenePath.empty())
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		else
			SaveSceneAs();
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Pine Scene (*.pine)\0*.pine\0");
		if (!filepath.empty())
		{
			SerializeScene(m_ActiveScene, filepath);
			m_EditorScenePath = filepath;
		}
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneImporter::SaveScene(scene, path);
	}

	void EditorLayer::OnScenePlay()
	{
		if (m_SceneState == SceneState::Simulate)
			OnSceneStop();

		m_SceneState = SceneState::Play;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneSimulate()
	{
		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		m_SceneState = SceneState::Simulate;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		PN_CORE_ASSERT(m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate, "Scene not being played or simulated!");

		if (m_SceneState == SceneState::Play)
			m_ActiveScene->OnRuntimeStop();
		else if (m_SceneState == SceneState::Simulate)
			m_ActiveScene->OnSimulationStop();

		m_SceneState = SceneState::Edit;

		m_ActiveScene->OnRuntimeStop();
		m_ActiveScene = m_EditorScene;

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnScenePause()
	{
		if (m_SceneState == SceneState::Edit)
			return;

		m_ActiveScene->SetPaused(true);
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
		{
			Entity newEntity = m_EditorScene->DuplicateEntity(selectedEntity);
			m_SceneHierarchyPanel.SetSelectedEntity(newEntity);
		}
	}
}
