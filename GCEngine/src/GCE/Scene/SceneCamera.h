#pragma once

#include "GCE/Renderer/Camera.h"

namespace GCE
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		//ORTHOGRAPHIC
		void setOrthographic(float size, float nearClip, float farClip);

		float getOrthographicSize() const { return m_OrthographicSize; }
		void setOrthographicSize(float orthographicSize) { m_OrthographicSize = orthographicSize; recalculateProjection(); }

		float getOrthographicNearClip() const { return m_OrthographicNear; }
		void setOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; m_OrthographicNear = nearClip; recalculateProjection(); }

		float getOrthographicFarClip() const { return m_OrthographicFar; }
		void setOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; recalculateProjection(); }
		//------------


		//PERSPECTIVE
		void setPerspective(float verticalFOV, float nearClip, float farClip);

		float getPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void setPerspectiveVerticalFOV(float verticalFOV) { m_PerspectiveFOV = verticalFOV; recalculateProjection(); }

		float getPerspectiveHorizontalFOV() const { return m_PerspectiveFOV; }
		void setPerspectiveHorizontalFOV(float horizontalFOV) { m_PerspectiveFOV = horizontalFOV; recalculateProjection(); }

		float getPerspectiveNearClip() const { return m_PerspectiveNear; }
		void setPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; recalculateProjection(); }

		float getPerspectiveFarClip() const { return m_PerspectiveFar; }
		void setPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; recalculateProjection(); }
		//------------


		ProjectionType const getProjectionType() const { return m_ProjectionType; }
		void setProjectionType(ProjectionType type) { m_ProjectionType = type; recalculateProjection(); }
		
		void setViewportSize(unsigned int width, unsigned int height);

	private:
		void recalculateProjection();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.1f;
		float m_PerspectiveFar = 1000.0f;

		float m_AspectRatio = 16.0f / 9.0f;
	};
}
