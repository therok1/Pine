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

	m_Texture = Pine::Texture2D::Create("assets/textures/checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	PN_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Pine::Timestep ts)
{
	PN_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	Pine::Renderer2D::ResetStats();
	{
		PN_PROFILE_SCOPE("Renderer Prep");

		Pine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Pine::RenderCommand::Clear();
	}

	{
		PN_PROFILE_SCOPE("Renderer Draw");


		static float rot = 0.0f;
		rot += ts * 20.0f;

		Pine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Pine::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(rot), { 0.8f, 0.2f, 0.3f, 1.0f });
		Pine::Renderer2D::DrawQuad({ 0.6f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Pine::Renderer2D::DrawQuad({ 0.0f, -0.5f }, { 0.5f, 0.75f }, { 0.9f, 0.3f, 0.8f, 1.0f });
		Pine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_Texture, 10.0f);
		Pine::Renderer2D::EndScene();

		Pine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec3 color = glm::vec3((x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f);
				Pine::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, glm::vec4(color, 0.7f));
			}
		}
		Pine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	PN_PROFILE_FUNCTION();

	ImGui::Begin("Stats");

	auto stats = Pine::Renderer2D::GetStats();
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();
}

void Sandbox2D::OnEvent(Pine::Event& event)
{
	m_CameraController.OnEvent(event);
}
