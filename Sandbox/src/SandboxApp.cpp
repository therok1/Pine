#include <Pine.h>

class ExampleLayer : public Pine::Layer
{
public:

	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		PN_INFO("ExampleLayer::Update");
	}

	void OnEvent(Pine::Event& event) override
	{
		PN_TRACE("{0}", event);
	}

};

class Sandbox : public Pine::Application
{
public:

	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Pine::ImGuiLayer());
	}

	~Sandbox()
	{

	}

};

Pine::Application* Pine::CreateApplication()
{
	return new Sandbox();
}