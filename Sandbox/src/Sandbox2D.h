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

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

private:

	std::vector<ProfileResult> m_ProfileResults;

	Pine::OrthographicCameraController m_CameraController;

	Pine::Ref<Pine::Texture2D> m_Texture;

	glm::vec4 m_Color = { 0.0f, 0.0f, 0.0f, 1.0f };

};