#include "GCEPCH.h"
#include "ScriptGlue.h"

#include "GCE/Scripting/ScriptEngine.h"

#include "GCE/Core/UUID.h"
#include "GCE/Core/KeyCodes.h"
#include "GCE/Core/Input.h"
#include "GCE/Scene/Scene.h"
#include "GCE/Scene/Entity.h"

#include <glm/glm.hpp>
#include <box2d/b2_body.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>


namespace GCE
{
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define GCE_ADD_INTERNAL_CALL(name) mono_add_internal_call("GCE.InternalCalls::" #name, name)

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::getSceneContext();
		GCE_CORE_ASSERT(scene, "Scene not loaded");
		Entity entity = scene->getEntityByUUID(entityID);
		GCE_CORE_ASSERT(entity, "Entity not loaded");

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		GCE_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end(), "Component not found");
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::getSceneContext();
		GCE_CORE_ASSERT(scene, "Scene not loaded");
		Entity entity = scene->getEntityByUUID(entityID);
		GCE_CORE_ASSERT(entity, "Entity not loaded");

		*outTranslation = entity.getComponent<TransformComponent>().translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::getSceneContext();
		GCE_CORE_ASSERT(scene, "Scene not loaded");
		Entity entity = scene->getEntityByUUID(entityID);
		GCE_CORE_ASSERT(entity, "Entity not loaded");

		entity.getComponent<TransformComponent>().translation = *translation;
	}

	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::getSceneContext();
		GCE_CORE_ASSERT(scene, "Scene not loaded");
		Entity entity = scene->getEntityByUUID(entityID);
		GCE_CORE_ASSERT(entity, "Entity not loaded");

		auto& rb2d = entity.getComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.runtimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::getSceneContext();
		GCE_CORE_ASSERT(scene, "Scene not loaded");
		Entity entity = scene->getEntityByUUID(entityID);
		GCE_CORE_ASSERT(entity, "Entity not loaded");

		auto& rb2d = entity.getComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.runtimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::isKeyPressed(keycode);
	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
		{
			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);
			std::string managedTypename = fmt::format("GCE.{}", structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::getCoreAssemblyImage());
			if (!managedType)
			{
				GCE_CORE_ERROR("Could not find component type {}", managedTypename);
				return;
			}
			s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.hasComponent<Component>(); };
		}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::registerComponents()
	{
		RegisterComponent(AllComponents{});
	}


	void ScriptGlue::registerFunctions()
	{
		GCE_ADD_INTERNAL_CALL(Entity_HasComponent);
		GCE_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		GCE_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		GCE_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		GCE_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);

		GCE_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}

}