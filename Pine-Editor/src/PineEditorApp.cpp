#include <Pine.h>
#include <Pine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Pine
{
	class PineEditor : public Application
	{
	public:

		PineEditor(const ApplicationSpecification& specification)
			: Application(specification)
		{
			PushLayer(new EditorLayer());
		}

		~PineEditor()
		{

		}

	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Pine-Editor";
		spec.CommandLineArgs = args;

		return new PineEditor(spec);
	}
}
