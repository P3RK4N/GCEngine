#pragma once

#include "GCE/Scene/SceneCamera.h"
#include "GCE/Scene/ScriptableEntity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

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
			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 }) *	
							glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 }) *	
							glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 });

			return	glm::translate(glm::mat4(1.0f), translation) *
					rot *
					glm::scale(glm::mat4(1.0f), scale);
		}
	};
	
	struct SpriteRendererComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

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
}