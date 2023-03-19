#include "pnpch.h"
#include "OrthographicCameraController.h"

#include "Pine/Core/Input.h"
#include "Pine/Core/KeyCodes.h"

namespace Pine
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:
		m_AspectRatio(aspectRatio),
		m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }),
		m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top),
		m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		PN_PROFILE_FUNCTION();

		float cameraTranslationSpeed = m_CameraTranslationSpeed * ts;
		float cameraRotationSpeed = m_CameraRotationSpeed * ts;
		glm::vec2 move = glm::vec2(0.0f);

		if (Input::IsKeyPressed(Key::A))
			move.x -= 1.0f;
		if (Input::IsKeyPressed(Key::D))
			move.x += 1.0f;
		if (Input::IsKeyPressed(Key::W))
			move.y += 1.0f;
		if (Input::IsKeyPressed(Key::S))
			move.y -= 1.0f;

		if (glm::length(move))
			move = glm::normalize(move);
		
		m_CameraPosition.x += move.x * cos(glm::radians(m_CameraRotation)) * cameraTranslationSpeed;
		m_CameraPosition.y += move.x * sin(glm::radians(m_CameraRotation)) * cameraTranslationSpeed;
		m_CameraPosition.x += -move.y * sin(glm::radians(m_CameraRotation)) * cameraTranslationSpeed;
		m_CameraPosition.y += move.y * cos(glm::radians(m_CameraRotation)) * cameraTranslationSpeed;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation += cameraRotationSpeed;
			if (Input::IsKeyPressed(Key::E))
				m_CameraRotation -= cameraRotationSpeed;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		PN_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(PN_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(PN_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateView();
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		PN_PROFILE_FUNCTION();

		m_ZoomLevel -= event.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateView();

		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& event)
	{
		PN_PROFILE_FUNCTION();

		OnResize(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));

		return false;
	}
}
