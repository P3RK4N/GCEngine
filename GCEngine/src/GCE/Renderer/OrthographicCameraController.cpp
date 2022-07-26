#include "GCEPCH.h"
#include "GCE/Renderer/OrthographicCameraController.h"

#include "GCE/Core/Input.h"
#include "GCE/Core/KeyCodes.h"

namespace GCE
{
	
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) :
		m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::onUpdate(Timestep ts)
	{
		GCE_PROFILE_FUNCTION();

		//POSITION
		if (Input::isKeyPressed(GCE_KEY_A))
			m_Camera.setPosition(m_Camera.getPosition() - glm::vec3(m_CameraMoveSpeed * ts * m_ZoomLevel, 0.0f, 0.0f));
		if (Input::isKeyPressed(GCE_KEY_D))
			m_Camera.setPosition(m_Camera.getPosition() + glm::vec3(m_CameraMoveSpeed * ts * m_ZoomLevel, 0.0f, 0.0f));
		if (Input::isKeyPressed(GCE_KEY_S))
			m_Camera.setPosition(m_Camera.getPosition() - glm::vec3(0.0f, m_CameraMoveSpeed * ts * m_ZoomLevel, 0.0f));
		if (Input::isKeyPressed(GCE_KEY_W))
			m_Camera.setPosition(m_Camera.getPosition() + glm::vec3(0.0f, m_CameraMoveSpeed * ts * m_ZoomLevel, 0.0f));

		//ROTATION
		if (m_Rotation)
		{
			if (Input::isKeyPressed(GCE_KEY_Q))
				m_Camera.setRotation(m_Camera.getRotation() + m_CameraRotateSpeed * ts);
			if (Input::isKeyPressed(GCE_KEY_E))
				m_Camera.setRotation(m_Camera.getRotation() - m_CameraRotateSpeed * ts);
		}
	} 

	void OrthographicCameraController::onEvent(Event& e)
	{
		GCE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GCE_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(GCE_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
	}

	void OrthographicCameraController::onResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
	{
		GCE_PROFILE_FUNCTION();

		m_ZoomLevel = std::max(m_ZoomLevel - e.getYOffset() * m_ZoomSpeed, 0.25f);
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e)
	{
		GCE_PROFILE_FUNCTION();

		onResize((float)e.getWidth(), (float)e.getHeight());
		return false;
	}

}