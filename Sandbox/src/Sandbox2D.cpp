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
	m_VertexArray = Pine::VertexArray::Create();

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Pine::Ref<Pine::VertexBuffer> vertexBuffer = Pine::VertexBuffer::Create(vertices, sizeof(vertices));
	vertexBuffer->SetLayout({
		{ Pine::ShaderDataType::Float3, "a_Position" }
	});
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[] =
	{
		0, 1, 2, 2, 3, 0
	};

	Pine::Ref<Pine::IndexBuffer> indexBuffer = Pine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_Shader = Pine::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Pine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Pine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Pine::RenderCommand::Clear();

	Pine::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Pine::OpenGLShader>(m_Shader)->Bind();
	std::dynamic_pointer_cast<Pine::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_Color);

	Pine::Renderer::Submit(m_Shader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Pine::Renderer::EndScene();
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
