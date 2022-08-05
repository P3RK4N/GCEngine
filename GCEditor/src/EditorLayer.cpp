#include "EditorLayer.h"

#include <GCE/Core/EntryPoint.h>

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		m_FrameBuffer = FrameBuffer::create(spec);

		m_Scene = createRef<Scene>();

		m_Entity = m_Scene->createEntity("Square");
		m_Entity.addComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.2f, 1.0f));

		m_Camera = m_Scene->createEntity("Camera");
		m_Camera.addComponent<CameraComponent>();

		m_Camera.addComponent<NativeScriptComponent>().bind<CameraController>();

		m_SceneHierarchyPanel.setContext(m_Scene);
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
		}

		//Render
		m_FrameBuffer->bind();

		Renderer2D::resetStats();
		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::clear();

		m_Scene->onUpdate(ts);

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
			ImGui::Text("Menu Bar not added yet!");

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.onImGuiRender();

		draw();

		ImGui::End();
	}

	void EditorLayer::onEvent(Event& e)
	{

	}


	void EditorLayer::draw()
	{
		ImGui::Begin("Settings");

		auto stats = Renderer2D::getStats();
		ImGui::Text("Renderer2D stats:");
		ImGui::Text("Draw calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0});
		ImGui::Begin("Viewport");
		ImGui::PopStyleVar();

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::get()->getImGuiLayer()->setBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		auto newViewportSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = *(glm::vec2*)&(newViewportSize);

		unsigned int textureID = m_FrameBuffer->getColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
	}
}