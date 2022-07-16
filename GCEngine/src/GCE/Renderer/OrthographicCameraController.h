#pragma once

#include "GCE/Renderer/OrthographicCamera.h"
#include "GCE/Core/Timestep.h"

#include "GCE/Events/ApplicationEvent.h"
#include "GCE/Events/MouseEvent.h"

namespace GCE
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void onUpdate(Timestep ts);
		void onEvent(Event& e);

		OrthographicCamera& getCamera() { return m_Camera; }
		const OrthographicCamera& getCamera() const { return m_Camera; }

	private:
		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		float m_ZoomSpeed = 0.25f;
		OrthographicCamera m_Camera;

		bool m_Rotation;
		float m_CameraMoveSpeed = 1.0f;
		float m_CameraRotateSpeed = 90.0f;
	};
}
