#include "pnpch.h"
#include "Pine/Core/Input.h"

#include "Pine/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Pine
{
	bool Input::IsKeyPressed(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, static_cast<uint32_t>(keycode));
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(MouseButtonCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, static_cast<uint32_t>(button));
		return state == GLFW_PRESS;
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return std::pair<float, float>(static_cast<float>(xPos), static_cast<float>(yPos));
	}
}
