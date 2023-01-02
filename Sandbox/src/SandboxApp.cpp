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
		
	}

	void OnEvent(Pine::Event& event) override
	{
		
	}

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