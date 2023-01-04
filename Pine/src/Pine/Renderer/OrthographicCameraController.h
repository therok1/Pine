#pragma once

#include "Pine/Renderer/OrthographicCamera.h"

#include "Pine/Core/Timestep.h"

#include "Pine/Events/ApplicationEvent.h"
#include "Pine/Events/MouseEvent.h"

namespace Pine
{
	class OrthographicCameraController
	{
	public:

		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& event);

		OrthographicCamera& GetCamera()
		{
			return m_Camera;
		}

		const OrthographicCamera& GetCamera() const
		{
			return m_Camera;
		}

		float GetZoomLevel() const
		{
			return m_ZoomLevel;
		}

		void SetZoomLevel(float zoomLevel)
		{
			m_ZoomLevel = zoomLevel;
		}

	private:

		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

	private:

		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 90.0f;

	};
}