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

	EditorLayer::EditorLayer() :
		Layer("EditorLayer")
	{

	}

	void EditorLayer::onAttach()
	{
		GCE_PROFILE_FUNCTION();

		FrameBufferSpecification spec;
		spec.width = 1280;
		spec.height = 720;
		spec.attachmentSpecification = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };

		m_FrameBuffer = FrameBuffer::create(spec);
		m_Scene = createRef<Scene>();
		m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.01f, 1000.0f);
		m_SceneHierarchyPanel.setContext(m_Scene);

		auto commandLineArgs = Application::get()->getCommandLineArgs();
		if (commandLineArgs.count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(m_Scene);
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
			m_Scene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.setViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		if ((m_ViewportFocused || m_ViewportHovered) && !ImGuizmo::IsUsing())
		{
			m_EditorCamera.onUpdate(ts);
		}

		//Render
		m_FrameBuffer->bind();

		Renderer2D::resetStats();
		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::clear();

		m_FrameBuffer->clearColorAttachment(1,-1);

		m_Scene->onUpdateEditor(ts, m_EditorCamera);

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
			m_HoveredEntity = pixelData == -1 ? Entity{} : Entity{ (entt::entity)pixelData, m_Scene.get() };
		}

		m_FrameBuffer->unbind();
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
					saveScene();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.onImGuiRender();

		drawStats();
		drawViewport();

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
					saveScene();
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
		if (e.getMouseButton() == GCE_MOUSE_BUTTON_LEFT && !ImGuizmo::IsOver() && !Input::isKeyPressed(GCE_KEY_LEFT_ALT))
		{
			if (m_ViewportHovered)
				m_SceneHierarchyPanel.setSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	void EditorLayer::newScene()
	{
		m_Scene = createRef<Scene>();
		m_Scene->onViewportResize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
		m_SceneHierarchyPanel.setContext(m_Scene);
	}

	void EditorLayer::openScene()
	{
		std::string filePath = FileDialogs::openFile("GCE Scene (*.gce)\0*.gce\0");
		if(!filePath.empty())
		{
			m_Scene = createRef<Scene>();
			m_Scene->onViewportResize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
			m_SceneHierarchyPanel.setContext(m_Scene);

			SceneSerializer serializer(m_Scene);
			serializer.deserialize(filePath);
		}
	}

	void EditorLayer::saveScene()
	{
		std::string filePath = FileDialogs::saveFile("GCE Scene (*.gce)\0*.gce\0");
		if (!filePath.empty())
		{
			SceneSerializer serializer(m_Scene);
			serializer.serialize(filePath);
		}
	}

	void EditorLayer::drawStats()
	{
		ImGui::Begin("Statistics");

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
			auto cameraEntity = m_Scene->getPrimaryCamera();
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

}