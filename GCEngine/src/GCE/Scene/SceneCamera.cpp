#include "GCEPCH.h"
#include "GCE/Scene/SceneCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace GCE
{

	SceneCamera::SceneCamera()
	{
		recalculateProjection();
	}

	void SceneCamera::setOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		setProjectionType(ProjectionType::Orthographic);
	}

	void SceneCamera::setPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;

		setProjectionType(ProjectionType::Perspective);
	}

	void SceneCamera::setViewportSize(unsigned int width, unsigned int height)
	{
		GCE_CORE_ASSERT(width > 0 && height > 0, "Cannot set viewport size to (0,0)");

		m_AspectRatio = (float)width / (float)height;

		recalculateProjection();
	}

	void SceneCamera::recalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Orthographic)
		{
			float orthoLeft = -0.5f * m_AspectRatio * m_OrthographicSize;
			float orthoRight = 0.5f * m_AspectRatio * m_OrthographicSize;
			float orthoDown = -0.5f * m_OrthographicSize;
			float orthoTop = 0.5f * m_OrthographicSize;

			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoDown, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
		else if(m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
	}
}