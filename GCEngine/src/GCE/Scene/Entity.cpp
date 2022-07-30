#include "GCEPCH.h"
#include "GCE/Scene/Entity.h"

namespace GCE
{

	Entity::Entity(entt::entity handle, Scene* scene) :
		m_EntityHandle(handle),
		m_Scene(scene)
	{

	}

}