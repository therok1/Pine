#pragma once

#include "Pine/Core/KeyCodes.h"
#include "Pine/Core/MouseButtonCodes.h"

namespace Pine
{
	class Input
	{
	public:

		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(MouseButtonCode button);

		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();

	};
}
