#include "pnpch.h"
#include "OrthographicCameraController.h"

#include "Pine/Core/Input.h"
#include "Pine/Core/KeyCodes.h"

namespace Pine
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: 
		m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		glm::vec3 change = { 0.0f, 0.0f, 0.0f };
		if (Input::IsKeyPressed(PN_KEY_A))
			change.x -= 1.0f;
		if (Input::IsKeyPressed(PN_KEY_D))
			change.x += 1.0f;
		if (Input::IsKeyPressed(PN_KEY_W))
			change.y += 1.0f;
		if (Input::IsKeyPressed(PN_KEY_S))
			change.y -= 1.0f;

		if (glm::length(change) > 0.0f)
			change = glm::normalize(change);

		change *= m_CameraTranslationSpeed * ts;
		m_CameraPosition += change;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(PN_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(PN_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(PN_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(PN_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_ZoomLevel -= event.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		m_AspectRatio = static_cast<float>(event.GetWidth()) / static_cast<float>(event.GetHeight());
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}
}