#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: 
	Layer("Sandbox2D"),
	m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	PN_PROFILE_FUNCTION();

	m_Texture = Pine::Texture2D::Create("assets/textures/texture.png");
}

void Sandbox2D::OnDetach()
{
	PN_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Pine::Timestep ts)
{
	PN_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	{
		PN_PROFILE_SCOPE("Renderer Prep");

		Pine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Pine::RenderCommand::Clear();
	}

	{
		PN_PROFILE_SCOPE("Renderer Draw");

		Pine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//Pine::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
		Pine::Renderer2D::DrawQuad({ 0.6f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Pine::Renderer2D::DrawQuad({ 0.0f, -0.5f }, { 0.5f, 0.75f }, { 0.9f, 0.3f, 0.8f, 1.0f });
		//Pine::Renderer2D::DrawQuad({ 0.2f, 0.5f }, { 0.5f, 0.5f }, m_Texture, 1.0f, glm::vec4(1.0f, 0.2f, 0.2f, 1.0f));
		Pine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	PN_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_Color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Pine::Event& event)
{
	m_CameraController.OnEvent(event);
}
