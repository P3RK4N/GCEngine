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
		OrthographicCameraController m_CameraController;

		//TEMP
		Ref<FrameBuffer> m_FrameBuffer;
		glm::vec2 m_ViewportSize = {0, 0};
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		Ref<Scene> m_Scene;

		Ref<Texture2D> texture;
		Ref<Texture2D> spriteSheet;
		Ref<SubTexture2D> stairs;

		glm::vec4 m_Color = glm::vec4(1.0f);
		glm::vec2 m_Position = glm::vec2(0.0f);
		glm::vec2 m_Scale = glm::vec2(1.0f);
		float m_Rotation = 0.0f;
		int m_TextureScale = 1;

		float m_DragSpeed = 0.1f;

		Entity m_Entity;
		Entity m_Camera;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;

	};
}