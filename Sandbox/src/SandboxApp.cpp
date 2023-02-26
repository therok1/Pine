#include <Pine.h>
#include <Pine/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Pine::Application
{
public:

	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}

};

Pine::Application* Pine::CreateApplication(ApplicationCommandLineArgs args)
{
	return new Sandbox();
}
