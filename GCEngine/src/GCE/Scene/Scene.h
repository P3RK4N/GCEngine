#pragma once

#include "GCE/Core/Timestep.h"

#include <entt.hpp>

namespace GCE
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity createEntity(const std::string& name = "");

		void onUpdate(Timestep ts);
		void onViewportResize(unsigned int width, unsigned int height);

	private:
		void resetAspectRatio();

	private:
		entt::registry m_Registry;

		unsigned int m_ViewportWidth;
		unsigned int m_ViewportHeight;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}