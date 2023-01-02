#include <Pine.h>

class ExampleLayer : public Pine::Layer
{
public:

	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray = std::shared_ptr<Pine::VertexArray>(Pine::VertexArray::Create());

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Pine::VertexBuffer> vertexBuffer = std::shared_ptr<Pine::VertexBuffer>(Pine::VertexBuffer::Create(vertices, sizeof(vertices)));
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

		std::shared_ptr<Pine::IndexBuffer> indexBuffer = std::shared_ptr<Pine::IndexBuffer>(Pine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = std::shared_ptr<Pine::VertexArray>(Pine::VertexArray::Create());

		float squareVertices[] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Pine::VertexBuffer> squareVB = std::shared_ptr<Pine::VertexBuffer>(Pine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({ { Pine::ShaderDataType::Float3, "a_Position" } });
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[] =
		{
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<Pine::IndexBuffer> squareIB = std::shared_ptr<Pine::IndexBuffer>(Pine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;
			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_Shader = std::make_shared<Pine::Shader>(vertexSource, fragmentSource);

		std::string squareVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}

		)";

		std::string squareFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}

		)";

		m_SquareShader = std::make_shared<Pine::Shader>(squareVertexSource, squareFragmentSource);
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

		Pine::Renderer::Submit(m_SquareShader, m_SquareVA);
		Pine::Renderer::Submit(m_Shader, m_VertexArray);

		Pine::Renderer::EndScene();
	}

	void OnEvent(Pine::Event& event) override
	{

	}

private:

	std::shared_ptr<Pine::Shader> m_Shader;
	std::shared_ptr<Pine::VertexArray> m_VertexArray;

	std::shared_ptr<Pine::Shader> m_SquareShader;
	std::shared_ptr<Pine::VertexArray> m_SquareVA;

	Pine::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 2.5f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 90.0f;

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