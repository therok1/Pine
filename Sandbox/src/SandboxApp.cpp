#include <Pine.h>
#include <Pine/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Pine::Application
{
public:

	Sandbox(const Pine::ApplicationSpecification& specification)
		: Pine::Application(specification)
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}

};

Pine::Application* Pine::CreateApplication(ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Pine-Editor";
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}
