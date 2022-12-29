#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Pine
{
	class PINE_API Application
	{
	public:

		Application();
		virtual ~Application();

		void Run();

	};

	Application* CreateApplication();
}

