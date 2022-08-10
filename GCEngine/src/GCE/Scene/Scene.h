#pragma once

#include "GCE/Core/Timestep.h"
#include "GCE/Renderer/EditorCamera.h"

#include <entt.hpp>

namespace GCE
{
	class Entity;
	struct CameraComponent;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity createEntity(const std::string& name = "");
		void destroyEntity(Entity entity);

		void onUpdateEditor(Timestep ts, EditorCamera& camera);
		void onUpdateRuntime(Timestep ts);
		void onViewportResize(unsigned int width, unsigned int height);

		Entity getPrimaryCamera();

	private:
		void resetCameraComponent(CameraComponent& cameraComponent);

		template<typename T>
		void onComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;

		unsigned int m_ViewportWidth;
		unsigned int m_ViewportHeight;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}