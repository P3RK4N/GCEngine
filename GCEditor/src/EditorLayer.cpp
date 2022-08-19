#include "EditorLayer.h"

#include <GCE/Core/EntryPoint.h>
#include <GCE/Scene/SceneSerializer.h>
#include <GCE/Utils/PlatformUtils.h>

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include <GCE/Math/Math.h>

namespace GCE
{
	class CameraController : public ScriptableEntity
	{
	public:
		void onCreate()
		{

		}

		void onDestroy()
		{

		}

		void onUpdate(Timestep ts)
		{
			auto& transform = getComponent<TransformComponent>();
			float speed = 5.0f;

			if (Input::isKeyPressed(GCE_KEY_A))
				transform.translation[0] -= speed * ts;
			if (Input::isKeyPressed(GCE_KEY_D))
				transform.translation[0] += speed * ts;
			if (Input::isKeyPressed(GCE_KEY_S))
				transform.translation[1] -= speed * ts;
			if (Input::isKeyPressed(GCE_KEY_W))
				transform.translation[1] += speed * ts;
		}
	};

	extern const std::filesystem::path g_AssetPath = "assets";

	EditorLayer::EditorLayer() :
		Layer("EditorLayer")
	{

	}

	void EditorLayer::onAttach()
	{
		GCE_PROFILE_FUNCTION();

		m_IconPlay = Texture2D::create("resources/icons/Play.png");
		m_IconStop = Texture2D::create("resources/icons/Stop.png");

		FrameBufferSpecification spec;
		spec.width = 1280;
		spec.height = 720;
		spec.attachmentSpecification = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };

		m_FrameBuffer = FrameBuffer::create(spec);
		m_ActiveScene = createRef<Scene>();
		m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.01f, 1000.0f);
		m_SceneHierarchyPanel.setContext(m_ActiveScene);

		auto commandLineArgs = Application::get()->getCommandLineArgs();
		if (commandLineArgs.count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(m_ActiveScene);
			serializer.deserialize(sceneFilePath);
		}
	}

	void EditorLayer::onDetach()
	{
		GCE_PROFILE_FUNCTION();

	}

	void EditorLayer::onUpdate(Timestep ts)
	{
		GCE_PROFILE_FUNCTION();

		//Update viewport
		FrameBufferSpecification spec = m_FrameBuffer->getSpecification();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
		{
			m_FrameBuffer->resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.setViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}


		//Render
		m_FrameBuffer->bind();

		Renderer2D::resetStats();
		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::clear();

		m_FrameBuffer->clearColorAttachment(1,-1);

		switch (m_SceneState)
		{
			case SceneState::Edit:
				{
					if ((m_ViewportFocused || m_ViewportHovered) && !ImGuizmo::IsUsing())
					{
						m_EditorCamera.onUpdate(ts);
					}
					m_ActiveScene->onUpdateEditor(ts, m_EditorCamera);
					break;
				}
			case SceneState::Play:
				{
					m_ActiveScene->onUpdateRuntime(ts);
					break;
				}
		}


		auto[mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		//Flip
		my = viewportSize.y - my;

		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_FrameBuffer->readPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity{} : Entity{ (entt::entity)pixelData, m_ActiveScene.get() };
		}

		onOverlayRender();

		m_FrameBuffer->unbind();
	}

	void EditorLayer::onOverlayRender()
	{

		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->getPrimaryCamera();
			
			Renderer2D::beginScene(camera.getComponent<CameraComponent>().camera, camera.getComponent<TransformComponent>().getTransform());
		}
		else
		{
			Renderer2D::beginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{
			{
				auto view = m_ActiveScene->getAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc , cc2dc] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					glm::vec3 translation = tc.translation + glm::vec3(cc2dc.offset, 0.01f);
					glm::vec3 scale = tc.scale * glm::vec3(cc2dc.radius * 2.0f);

					glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), translation), scale);

					Renderer2D::drawCircle(transform, glm::vec4(0,1,1,1), 0.05f);
				}
			}

			{
				auto view = m_ActiveScene->getAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc , bc2dc] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					glm::vec3 translation = tc.translation + glm::vec3(bc2dc.offset, 0.01f);
					glm::vec3 scale = tc.scale * glm::vec3(bc2dc.size * 2.0f, 1.0f);

					glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), translation), tc.rotation.z, {0,0,1}), scale); 

					Renderer2D::drawRect(transform, glm::vec4(0,1,1,1));
				}
			}
		}

		Renderer2D::endScene();
	}

	void EditorLayer::onImGuiRender()
	{
		GCE_PROFILE_FUNCTION();

		static bool dockSpaceOpen = true;

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		
		ImGuiStyle& style = ImGui::GetStyle();
		float prevSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = prevSize;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					newScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					openScene();
				}

				if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
				{
					saveSceneAs();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.onImGuiRender();
		m_ContentBrowserPanel.onImGuiRender();

		drawStats();
		drawViewport();

		UI_Toolbar();

		ImGui::End();
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;

		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		if (ImGui::ImageButton((ImTextureID)icon->getRendererID(), ImVec2{ size, size }, ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_SceneState == SceneState::Edit)
				onScenePlay();
			else if (m_SceneState == SceneState::Play)
				onSceneStop();
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::onEvent(Event& e)
	{
		m_EditorCamera.onEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(GCE_BIND_EVENT_FN(EditorLayer::onKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(GCE_BIND_EVENT_FN(EditorLayer::onMouseButtonPressed));
	}

	bool EditorLayer::onKeyPressed(KeyPressedEvent& e)
	{
		if(e.getRepeatCount() > 0)
			return false;

		bool control = Input::isKeyPressed(GCE_KEY_LEFT_CONTROL) || Input::isKeyPressed(GCE_KEY_RIGHT_CONTROL);
		bool shift = Input::isKeyPressed(GCE_KEY_LEFT_SHIFT) || Input::isKeyPressed(GCE_KEY_LEFT_SHIFT);

		switch (e.getKeyCode())
		{
			case GCE_KEY_N:
				if(control)
					newScene();
				break;

			case GCE_KEY_O:
				if(control)
					openScene();
				break;

			case GCE_KEY_S:
				if(control && shift)
					saveSceneAs();
				else if (control)
					saveScene();
				break;


			case GCE_KEY_D:
				if (control)
					onDuplicateEntity();
					break;

			//Gizmos
			case GCE_KEY_Q:
				m_GizmoType = -1;
				break;
			case GCE_KEY_W:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case GCE_KEY_E:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case GCE_KEY_R:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}
	}

	bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.getMouseButton() == GCE_MOUSE_BUTTON_LEFT && (!ImGuizmo::IsOver() || !m_SceneHierarchyPanel.getSelectedEntity()) && !Input::isKeyPressed(GCE_KEY_LEFT_ALT))
		{
			if (m_ViewportHovered)
				m_SceneHierarchyPanel.setSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	void EditorLayer::serializeScene(const Ref<Scene>& scene, const std::filesystem::path& filePath)
	{
		SceneSerializer serializer(scene);
		serializer.serialize(filePath.string());
	}

	void EditorLayer::newScene()
	{
		m_ActiveScene = createRef<Scene>();
		m_ActiveScene->onViewportResize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
		m_SceneHierarchyPanel.setContext(m_ActiveScene);
		m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::openScene()
	{
		std::string filePath = FileDialogs::openFile("GCE Scene (*.gce)\0*.gce\0");
		if(!filePath.empty())
			openScene(filePath);
	}

	void EditorLayer::openScene(const std::filesystem::path& filePath)
	{
		if (m_SceneState != SceneState::Edit)
			onSceneStop();

		if (filePath.extension().string() != ".gce")
			return;

		m_EditorScene = createRef<Scene>();
		m_EditorScene->onViewportResize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
		m_SceneHierarchyPanel.setContext(m_EditorScene);

		SceneSerializer serializer(m_EditorScene);
		serializer.deserialize(filePath.string());

		m_ActiveScene = m_EditorScene;
		m_EditorScenePath = filePath;
	}

	void EditorLayer::saveSceneAs()
	{
		std::string filePath = FileDialogs::saveFile("GCE Scene (*.gce)\0*.gce\0");
		if (!filePath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.serialize(filePath);
			m_EditorScenePath = filePath;
		}
	}

	void EditorLayer::saveScene()
	{
		if (!m_EditorScenePath.empty())
		{
			SceneSerializer serializer(m_EditorScene);
			serializer.serialize(m_EditorScenePath.string());
		}
		else
			saveSceneAs();
	}

	void EditorLayer::drawStats()
	{
		ImGui::Begin("Statistics");

		ImGui::Checkbox("Show Physics Collides", &m_ShowPhysicsColliders);
		std::string hoveredEntity = !m_HoveredEntity ? "None" : m_HoveredEntity.getComponent<TagComponent>().tag;
		ImGui::Text("Hovered Entity: %s", hoveredEntity.c_str());

		auto stats = Renderer2D::getStats();
		ImGui::Text("Renderer2D stats:");
		ImGui::Text("Draw calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());

		ImGui::End();
	}

	void EditorLayer::drawViewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0});
		ImGui::Begin("Viewport");
		ImGui::PopStyleVar();

		//Mouse picking
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };


		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::get()->getImGuiLayer()->setBlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		auto newViewportSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = *(glm::vec2*)&(newViewportSize);

		unsigned int textureID = m_FrameBuffer->getColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_ASSET"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				openScene(std::filesystem::path(g_AssetPath / path));
			}
			ImGui::EndDragDropTarget();
		}

		if (m_SceneState == SceneState::Edit)
			drawGuizmo();

		ImGui::End();
	}

	void EditorLayer::drawGuizmo()
	{
		Entity selectedEntity = m_SceneHierarchyPanel.getSelectedEntity();

		if(selectedEntity and m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			//Camera
#if SceneCamera
			//SceneCamera
			auto cameraEntity = m_ActiveScene->getPrimaryCamera();
			const auto& camera = cameraEntity.getComponent<CameraComponent>().camera;

			//Matrices
			const glm::mat4& cameraProjection = camera.getProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.getComponent<TransformComponent>().getTransform());
#else
			//EditorCamera
			//Matrices
			const glm::mat4& cameraProjection = m_EditorCamera.getProjection();
			glm::mat4 cameraView = m_EditorCamera.getViewMatrix();
#endif

			//Entity
			auto& entityTransformComponent = selectedEntity.getComponent<TransformComponent>();
			glm::mat4 transform = entityTransformComponent.getTransform();

			//Snapping
			bool snap = Input::isKeyPressed(GCE_KEY_LEFT_CONTROL);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };


			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - entityTransformComponent.rotation;

				entityTransformComponent.translation = translation;
				entityTransformComponent.rotation += deltaRotation;
				entityTransformComponent.scale = scale;
			}
		}
	}

	void EditorLayer::onScenePlay()
	{
		m_SceneState = SceneState::Play;

		m_ActiveScene = Scene::copy(m_EditorScene);
		m_SceneHierarchyPanel.setContext(m_ActiveScene);
		m_ActiveScene->onRuntimeStart();
	}

	void EditorLayer::onSceneStop()
	{
		m_SceneState = SceneState::Edit;
		m_ActiveScene->onRuntimeStop();

		m_ActiveScene = m_EditorScene;
		m_SceneHierarchyPanel.setContext(m_ActiveScene);
		m_ActiveScene->onViewportResize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
	}

	void EditorLayer::onDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		Entity selectedEntity = m_SceneHierarchyPanel.getSelectedEntity();

		if (selectedEntity)
			m_EditorScene->duplicateEntity(selectedEntity);
	}

}