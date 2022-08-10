#include "GCEPCH.h"
#include "GCE/Renderer/EditorCamera.h"

#include "GCE/Core/Input.h"
#include "GCE/Core/KeyCodes.h"
#include "GCE/Core/MouseButtonCodes.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace GCE
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip) :
	m_FOV(fov),
	m_AspectRatio(aspectRatio),
	m_NearClip(nearClip),
	m_FarClip(farClip),
	Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		updateView();
	}


	void EditorCamera::onUpdate(Timestep ts)
	{
		if(Input::isKeyPressed(GCE_KEY_LEFT_ALT))
		{
			const glm::vec2& mouse{ Input::getMouseX(), Input::getMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::isMouseButtonPressed(GCE_MOUSE_BUTTON_MIDDLE))
				mousePan(delta);
			else if (Input::isMouseButtonPressed(GCE_MOUSE_BUTTON_LEFT))
				mouseRotate(delta);
			else if (Input::isMouseButtonPressed(GCE_MOUSE_BUTTON_RIGHT))
				mouseZoom(delta.y);
		}

		updateView();
	}

	void EditorCamera::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GCE_BIND_EVENT_FN(EditorCamera::onMouseScroll));
	}

	bool EditorCamera::onMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.getYOffset() * 0.1f;
		mouseZoom(delta);
		updateView();
		return false;
	}

	float EditorCamera::zoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}

	std::pair<float, float> EditorCamera::panSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f);
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f);
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::rotationSpeed() const
	{
		return 0.8f;
	}

	void EditorCamera::updateView()
	{
		m_Position = calculatePosition();

		glm::quat orientation = getOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	void EditorCamera::updateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;

		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}


	glm::vec3 EditorCamera::getUpDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::getRightDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::getForwardDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::calculatePosition() const
	{
		return m_FocalPoint - getForwardDirection() * m_Distance;
	}

	glm::quat EditorCamera::getOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}


	void EditorCamera::mouseZoom(float delta)
	{
		m_Distance -= delta * zoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += getForwardDirection();
			m_Distance = 1.0f;
		}
	}

	void EditorCamera::mousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = panSpeed();
		m_FocalPoint += -getRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += getUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::mouseRotate(const glm::vec2& delta)
	{
		float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * rotationSpeed();
		m_Pitch += delta.y * rotationSpeed();
	}

}