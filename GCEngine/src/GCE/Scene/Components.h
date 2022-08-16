#pragma once

#include "GCE/Scene/SceneCamera.h"
#include "GCE/Scene/ScriptableEntity.h"
#include "GCE/Renderer/Texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace GCE
{
	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : tag(tag) {}

		operator const std::string& () { return tag; }
	};

	struct TransformComponent
	{
		glm::vec3 translation{ 0.0f };
		glm::vec3 rotation{ 0.0f };
		glm::vec3 scale{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3 translation) : translation(translation) {}

		glm::mat4 getTransform() const
		{
			glm::mat4 rot = glm::toMat4(glm::quat(rotation));

			return	glm::translate(glm::mat4(1.0f), translation) *
					rot *
					glm::scale(glm::mat4(1.0f), scale);
		}
	};
	
	struct SpriteRendererComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> texture;
		float textureScale = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera camera;
		bool primary = true;
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity*	(*instantiateScript)	();
		void				(*destroyScript)		(NativeScriptComponent*);

		template<typename T>
		void bind()
		{
			instantiateScript	=	[]	()								{ return static_cast<ScriptableEntity*>(new T());	};
			destroyScript		=	[]	(NativeScriptComponent* nsc)	{ delete nsc->instance; nsc->instance = nullptr;	};
		}
	};

	//Physics
	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType type = BodyType::Dynamic;
		bool fixedRotation = false;

		void* runtimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 offset = { 0.0f, 0.0f };
		glm::vec2 size = { 0.5f, 0.5f };

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.5f;
		float restitutionThreshold = 0.5f;

		void* runtimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};
}