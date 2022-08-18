#pragma once

#include "GCE/Core/Timestep.h"
#include "GCE/Renderer/EditorCamera.h"
#include "GCE/Core/UUID.h"

#include <entt.hpp>

class b2World;

namespace GCE
{
	class Entity;
	struct CameraComponent;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> copy(Ref<Scene> otherScene);

		Entity createEntity(const std::string& name = "");
		Entity createEntityWithUUID(UUID uuid, const std::string& name = "");
		void destroyEntity(Entity entity);

		void onRuntimeStart();
		void onRuntimeStop();

		void onUpdateEditor(Timestep ts, EditorCamera& camera);
		void onUpdateRuntime(Timestep ts);
		void onViewportResize(unsigned int width, unsigned int height);

		void duplicateEntity(Entity entity);

		Entity getPrimaryCamera();

	private:
		void resetCameraComponent(CameraComponent& cameraComponent);

		template<typename T>
		void onComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;

		unsigned int m_ViewportWidth;
		unsigned int m_ViewportHeight;

		b2World* m_PhysicsWorld = nullptr;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}