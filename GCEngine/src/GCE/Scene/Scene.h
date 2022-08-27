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

		void onSimulationStart();
		void onSimulationStop();

		void onUpdateEditor(Timestep ts, const EditorCamera& camera);
		void onUpdateRuntime(Timestep ts);
		void onUpdateSimulation(Timestep ts, const EditorCamera& camera);
		void onViewportResize(unsigned int width, unsigned int height);

		void duplicateEntity(Entity entity);

		Entity getPrimaryCamera();
		Entity getEntityByUUID(UUID uuid);

		template<typename... Components>
		auto getAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

		const unsigned int& getViewportWidth() { return m_ViewportWidth; }
		const unsigned int& getViewportHeight() { return m_ViewportHeight; }

	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);

		void onPhysics2DStart();
		void onPhysics2DStop();

		void renderScene(const EditorCamera& camera);

	private:
		entt::registry m_Registry;
		std::unordered_map<uint64_t, entt::entity> m_EntityMap;

		unsigned int m_ViewportWidth;
		unsigned int m_ViewportHeight;

		b2World* m_PhysicsWorld = nullptr;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}