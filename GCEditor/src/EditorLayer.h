#pragma once
#include "GCE.h"

#include "Panels/SceneHierarchyPanel.h"

namespace GCE
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void onUpdate(Timestep ts) override;
		virtual void onImGuiRender() override;
		virtual void onEvent(Event& e) override;

		virtual void onAttach() override;
		virtual void onDetach() override;

	public:
		void draw();

	private:

		//TEMP
		Ref<FrameBuffer> m_FrameBuffer;
		glm::vec2 m_ViewportSize = {0, 0};
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		Ref<Scene> m_Scene;

		Entity m_Entity;
		Entity m_Camera;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;

	};
}