#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

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
	
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Pine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Pine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Pine::RenderCommand::Clear();

	Pine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Pine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Pine::Renderer2D::EndScene();

	//std::dynamic_pointer_cast<Pine::OpenGLShader>(m_Shader)->Bind();
	//std::dynamic_pointer_cast<Pine::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_Color);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_Color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Pine::Event& event)
{
	m_CameraController.OnEvent(event);
}
