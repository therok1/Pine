#pragma once

#include "Pine/Core/Core.h"

namespace Pine
{
	class Input
	{
	public:

		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);

		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();

	};
}