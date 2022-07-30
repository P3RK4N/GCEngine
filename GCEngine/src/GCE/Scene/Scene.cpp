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

	void Scene::onUpdate(Timestep ts)
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
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if (camera.primary)
			{
				mainCamera = &camera.camera;
				cameraTransform = &transform.transform;
				break;
			}
		}
		if (mainCamera)
		{
			Renderer2D::beginScene(mainCamera->getProjection(), *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::drawQuad(transform, sprite.color);
			}

			Renderer2D::endScene();
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

	void Scene::resetAspectRatio()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cc = view.get<CameraComponent>(entity);
			if (!cc.fixedAspectRatio)
				cc.camera.setViewportSize(m_ViewportWidth, m_ViewportHeight);
		}
	}

}