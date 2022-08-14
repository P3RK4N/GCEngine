#include "GCEPCH.h"
#include "GCE/Scene/Scene.h"

#include "GCE/Scene/Entity.h"
#include "GCE/Scene/Components.h"
#include "GCE/Scene/ScriptableEntity.h"
#include "GCE/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace GCE
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::createEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };

		entity.addComponent<TransformComponent>();
		entity.addComponent<TagComponent>(name.empty() ? "Entity" : name);

		return entity;
	}

	void Scene::destroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::onUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::beginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			//Renderer2D::drawQuad(transform.getTransform(), sprite.color);
			Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
		}

		Renderer2D::endScene();
	}

	void Scene::onUpdateRuntime(Timestep ts)
	{
		//SCRIPTS
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			if (!nsc.instance)
			{
				nsc.instance = nsc.instantiateScript();
				nsc.instance->m_Entity = Entity{ entity, this };
				nsc.instance->onCreate();
			}

			nsc.instance->onUpdate(ts);
		});

		//RENDER 2D
		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if (camera.primary)
			{
				Camera* mainCamera = &camera.camera;
				glm::mat4 cameraTransform = transform.getTransform();

				Renderer2D::beginScene(*mainCamera, cameraTransform);

				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
					//Renderer2D::drawQuad(transform.getTransform(), sprite.color);
					Renderer2D::drawSprite(transform.getTransform(), sprite, (uint32_t)entity);
				}

				Renderer2D::endScene();

				break;
			}
		}
	}

	void Scene::onViewportResize(unsigned int width, unsigned int height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cc = view.get<CameraComponent>(entity);
			if (!cc.fixedAspectRatio)
				cc.camera.setViewportSize(width, height);
		}
	}

	void Scene::resetCameraComponent(CameraComponent& cameraComponent)
	{
		if (!cameraComponent.fixedAspectRatio)
				cameraComponent.camera.setViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	Entity Scene::getPrimaryCamera()
	{
		auto view = m_Registry.view<CameraComponent>();
		for(auto entity : view)
		{
			const auto& cameraComponent = view.get<CameraComponent>(entity);
			if (cameraComponent.primary)
				return Entity{entity, this};
		}
		return {};
	}


	template<typename T>
	void Scene::onComponentAdded(Entity, T& component)
	{
		//static_assert(false);
	}

	template<>
	void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.camera.setViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}
}