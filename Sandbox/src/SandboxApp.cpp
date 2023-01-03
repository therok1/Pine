#include <Pine.h>

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Pine::Layer
{
public:

	ExampleLayer()
		: Layer("Example"),
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
		m_CameraPosition(0.0f)
	{
		m_VertexArray = Pine::Ref<Pine::VertexArray>(Pine::VertexArray::Create());

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.3f, 0.0f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.5f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.9f, 0.1f, 0.2f, 1.0f
		};

		Pine::Ref<Pine::VertexBuffer> vertexBuffer = Pine::Ref<Pine::VertexBuffer>(Pine::VertexBuffer::Create(vertices, sizeof(vertices)));
		Pine::BufferLayout layout =
		{
			{ Pine::ShaderDataType::Float3, "a_Position" },
			{ Pine::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[] =
		{
			0, 1, 2
		};

		Pine::Ref<Pine::IndexBuffer> indexBuffer = Pine::Ref<Pine::IndexBuffer>(Pine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Pine::Ref<Pine::VertexArray>(Pine::VertexArray::Create());

		float squareVertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Pine::Ref<Pine::VertexBuffer> squareVB = Pine::Ref<Pine::VertexBuffer>(Pine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({ 
			{ Pine::ShaderDataType::Float3, "a_Position" }, 
			{ Pine::ShaderDataType::Float2, "a_TexCoord" } 
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[] =
		{
			0, 1, 2, 2, 3, 0
		};

		Pine::Ref<Pine::IndexBuffer> squareIB = Pine::Ref<Pine::IndexBuffer>(Pine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}

		)";

		m_Shader = Pine::Ref<Pine::Shader>(Pine::Shader::Create(vertexSource, fragmentSource));

		std::string flatColorVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string flatColorFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;
			
			void main()
			{
				color = vec4(u_Color, 1.0);
			}

		)";

		m_FlatColorShader = Pine::Ref<Pine::Shader>(Pine::Shader::Create(flatColorVertexSource, flatColorFragmentSource));

		m_TextureShader = Pine::Ref<Pine::Shader>(Pine::Shader::Create("assets/shaders/Texture.glsl"));

		m_Texture = Pine::Texture2D::Create("assets/textures/headshot.jpg");
		m_Texture2 = Pine::Texture2D::Create("assets/textures/texture.png");

		std::dynamic_pointer_cast<Pine::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Pine::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Pine::Timestep ts) override
	{
		if (Pine::Input::IsKeyPressed(PN_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		if (Pine::Input::IsKeyPressed(PN_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		if (Pine::Input::IsKeyPressed(PN_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		if (Pine::Input::IsKeyPressed(PN_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		
		if (Pine::Input::IsKeyPressed(PN_KEY_LEFT))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (Pine::Input::IsKeyPressed(PN_KEY_RIGHT))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Pine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Pine::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Pine::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Pine::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Pine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (std::size_t y = 0; y < 20; y++)
		{
			for (std::size_t x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Pine::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Pine::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_Texture2->Bind();
		Pine::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Pine::Renderer::Submit(m_Shader, m_VertexArray);

		Pine::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Pine::Event& event) override
	{

	}

private:

	Pine::Ref<Pine::Shader> m_Shader;
	Pine::Ref<Pine::VertexArray> m_VertexArray;

	Pine::Ref<Pine::Shader> m_FlatColorShader, m_TextureShader;
	Pine::Ref<Pine::VertexArray> m_SquareVA;

	Pine::Ref<Pine::Texture2D> m_Texture, m_Texture2;

	Pine::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 2.5f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_SquareColor = { 0.5f, 0.5f, 0.5f };

};

class Sandbox : public Pine::Application
{
public:

	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

Pine::Application* Pine::CreateApplication()
{
	return new Sandbox();
}