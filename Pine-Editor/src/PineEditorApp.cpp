#include <Pine.h>
#include <Pine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Pine
{
	class PineEditor : public Application
	{
	public:

		PineEditor()
			: Application("Pine Editor")
		{
			PushLayer(new EditorLayer());
		}

		~PineEditor()
		{

		}

	};

	Application* CreateApplication()
	{
		return new PineEditor();
	}
}