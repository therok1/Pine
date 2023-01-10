#pragma once

#include <Pine.h>

class Sandbox2D : public Pine::Layer
{
public:

	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Pine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Pine::Event& event) override;

private:

	Pine::OrthographicCameraController m_CameraController;

	Pine::Ref<Pine::Texture2D> m_Texture;
	Pine::Ref<Pine::Texture2D> m_SpriteSheet;

};