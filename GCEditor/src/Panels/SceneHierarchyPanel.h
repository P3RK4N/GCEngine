#pragma once

#include <GCE/Core/Core.h>
#include <GCE/Core/Log.h>
#include <GCE/Scene/Scene.h>
#include <GCE/Scene/Entity.h>

namespace GCE
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(const Ref<Scene>& scene);
		SceneHierarchyPanel() = default;

		void setContext(const Ref<Scene>& scene);

		void onImGuiRender();

		Entity getSelectedEntity() { return m_SelectionContext; }
		void setSelectedEntity(Entity pressedEntity) { m_SelectionContext = pressedEntity; }

	private:
		void drawEntityNode(Entity entity);
		void drawComponents(Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}