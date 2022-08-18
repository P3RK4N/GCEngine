#include "GCEPCH.h"
#include "GCE/Scene/Scene.h"

#include "GCE/Scene/Entity.h"
#include "GCE/Scene/Components.h"
#include "GCE/Scene/ScriptableEntity.h"
#include "GCE/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace GCE
{
	static b2BodyType GCERigidBody2DTypeToBox2D(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:	return b2_staticBody;
		case Rigidbody2DComponent::BodyType::Dynamic:	return b2_dynamicBody;
		case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		GCE_ASSERT(false, "Unknows body type!");
		return b2_staticBody;
	}

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	template<typename Component>
	static void copyComponent(entt::registry& src, entt::registry& dst, std::unordered_map<uint64_t, entt::entity>& enttMap)
	{
		auto view = src.view<Component>();
		for (auto e : view)
		{
			UUID uuid = src.get<IDComponent>(e).id;
			entt::entity enttID = enttMap.at(uuid);

			auto& component = src.get<Component>(e);
			dst.emplace_or_replace<Component>(enttID, component);
		}
	}

	template<typename Component>
	static void copyComponentIfExists(Entity src, Entity dst)
	{
		if (src.hasComponent<Component>())
			dst.addOrReplaceComponent<Component>(src.getComponent<Component>());
	}

	Ref<Scene> Scene::copy(Ref<Scene> otherScene)
	{
		Ref<Scene> newScene = createRef<Scene>();
		newScene->m_ViewportWidth = otherScene->m_ViewportWidth;
		newScene->m_ViewportHeight = otherScene->m_ViewportHeight;

		std::unordered_map<uint64_t, entt::entity> enttMap;

		auto& srcSceneRegistry = otherScene->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;

		auto idView = srcSceneRegistry.view<IDComponent>();
		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).id;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).tag;
			Entity newEntity = newScene->createEntityWithUUID(uuid, name);
			enttMap[uuid] = newEntity;
		}

		copyComponent<TransformComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);
		copyComponent<SpriteRendererComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);
		copyComponent<CircleRendererComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);
		copyComponent<CameraComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);
		copyComponent<NativeScriptComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);
		copyComponent<Rigidbody2DComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);
		copyComponent<BoxCollider2DComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);

		return newScene;
	}

	Entity Scene::createEntity(const std::string& name)
	{
		return createEntityWithUUID(UUID(), name);
	}

	Entity Scene::createEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };

		entity.addComponent<IDComponent>(uuid);
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

		{
			auto spriteView = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : spriteView)
			{
				auto [transform, sprite] = spriteView.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
			}
		}

		{
			auto circleView = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : circleView)
			{
				auto [transform, circle] = circleView.get<TransformComponent, CircleRendererComponent>(entity);
				Renderer2D::drawCircle(transform.getTransform(), circle.color, circle.thickness, circle.fade, (uint32_t)entity);
			}
		}

		Renderer2D::drawRect(glm::vec3(0.0f), glm::vec2(5.0f), glm::vec4(1.0f));

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

		//PHYSICS
		{
			const int velocityIterations = 6;
			const int positionIterations = 2;
			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto& e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.getComponent<TransformComponent>();
				auto& rb2d = entity.getComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rb2d.runtimeBody;
				const auto& position = body->GetPosition();

				transform.translation.x = position.x;
				transform.translation.y = position.y;
				transform.rotation.z = body->GetAngle();

			}
		}

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

				{
					auto spriteView = m_Registry.view<TransformComponent, SpriteRendererComponent>();
					for (auto entity : spriteView)
					{
						auto [transform, sprite] = spriteView.get<TransformComponent, SpriteRendererComponent>(entity);
						Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
					}
				}

				{
					auto circleView = m_Registry.view<TransformComponent, CircleRendererComponent>();
					for (auto entity : circleView)
					{
						auto [transform, circle] = circleView.get<TransformComponent, CircleRendererComponent>(entity);
						Renderer2D::drawCircle(transform.getTransform(), circle.color, circle.thickness, circle.fade, (uint32_t)entity);
					}
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

	void Scene::duplicateEntity(Entity entity)
	{
		std::string name = entity.getName();
		Entity newEntity = createEntity(name);

		copyComponentIfExists<TransformComponent>(entity, newEntity);
		copyComponentIfExists<SpriteRendererComponent>(entity, newEntity);
		copyComponentIfExists<CircleRendererComponent>(entity, newEntity);
		copyComponentIfExists<CameraComponent>(entity, newEntity);
		copyComponentIfExists<NativeScriptComponent>(entity, newEntity);
		copyComponentIfExists<Rigidbody2DComponent>(entity, newEntity);
		copyComponentIfExists<BoxCollider2DComponent>(entity, newEntity);
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

	void Scene::onRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.81f });
		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.getComponent<TransformComponent>();
			auto& rb2d = entity.getComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;

			bodyDef.type = GCERigidBody2DTypeToBox2D(rb2d.type);
			bodyDef.position.Set(transform.translation.x, transform.translation.y);
			bodyDef.angle = transform.rotation.z;


			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.fixedRotation);
			rb2d.runtimeBody = body;

			if(entity.hasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.getComponent<BoxCollider2DComponent>();

				b2PolygonShape polygonShape;
				polygonShape.SetAsBox(bc2d.size.x * transform.scale.x, bc2d.size.y * transform.scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &polygonShape;
				fixtureDef.density = bc2d.density;
				fixtureDef.friction = bc2d.friction;
				fixtureDef.restitution = bc2d.restitution;
				fixtureDef.restitutionThreshold = bc2d.restitutionThreshold;

				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::onRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
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
	void Scene::onComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{

	}

	template<>
	void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::onComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{

	}

	template<>
	void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}
	
	template<>
	void Scene::onComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{

	}
	
	template<>
	void Scene::onComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{

	}
}