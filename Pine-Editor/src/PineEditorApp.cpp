#include <Pine.h>
#include <Pine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Pine
{
	class PineEditor : public Application
	{
	public:

		PineEditor(ApplicationCommandLineArgs args)
			: Application("Pine Editor", args)
		{
			PushLayer(new EditorLayer());
		}

		~PineEditor()
		{

		}

	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new PineEditor(args);
	}
}
