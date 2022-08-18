#pragma once
#include "GCE.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

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
		void drawStats();
		void drawViewport();
		void drawGuizmo();

	private:
		bool onKeyPressed(KeyPressedEvent& e);
		bool onMouseButtonPressed(MouseButtonPressedEvent& e);

		void newScene();
		void openScene();
		void openScene(const std::filesystem::path& path);
		void saveSceneAs();
		void saveScene();

		void serializeScene(const Ref<Scene>& scene, const std::filesystem::path& filePath);

		void onScenePlay();
		void onSceneStop();

		void onDuplicateEntity();

		//UI
		void UI_Toolbar();

	private:
		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec2 m_ViewportSize = {0, 0};
		glm::vec2 m_ViewportBounds[2];

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		std::filesystem::path m_EditorScenePath;

		Entity m_HoveredEntity;

		int m_GizmoType = -1;

		EditorCamera m_EditorCamera;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		enum class SceneState
		{
			Edit = 0, Play = 1
		};

		Ref<Texture2D> m_IconPlay, m_IconStop;

		SceneState m_SceneState = SceneState::Edit;
	};
}