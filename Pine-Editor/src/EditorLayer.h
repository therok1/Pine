#pragma once

#include <Pine.h>

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

		OrthographicCameraController m_CameraController;

		Ref<Texture2D> m_Texture;
		Ref<Framebuffer> m_Framebuffer;

	};
}