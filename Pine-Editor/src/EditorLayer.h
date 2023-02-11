#pragma once

#include <Pine.h>

#include "Panels/SceneHierarchyPanel.h"

namespace Pine
{
	class EditorLayer : public Layer
	{
	public:

		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:

		bool OnKeyPressed(KeyPressedEvent& event);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

	private:

		OrthographicCameraController m_CameraController;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;

		Ref<Texture2D> m_Texture;
		Ref<Framebuffer> m_Framebuffer;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = glm::vec2(0.0f);

		int m_GizmoType = -1;

		SceneHierarchyPanel m_SceneHierarchyPanel;

	};
}
