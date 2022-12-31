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
		if (Pine::Input::IsKeyPressed(PN_KEY_TAB))
			PN_TRACE("Tab");
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
	}

	~Sandbox()
	{

	}

};

Pine::Application* Pine::CreateApplication()
{
	return new Sandbox();
}