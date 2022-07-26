#include "EditorLayer.h"

#include <GCE/Core/EntryPoint.h>

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GCE
{
	EditorLayer::EditorLayer() :
		Layer("EditorLayer"),
		m_CameraController(16.0f / 9.0f),
		texture(Texture2D::create("assets/textures/flowerPattern.jpg")),
		spriteSheet(Texture2D::create("assets/textures/spriteSheet.png"))
	{

	}

	void EditorLayer::onAttach()
	{
		GCE_PROFILE_FUNCTION();

		stairs = SubTexture2D::createFromCoords(spriteSheet, { 7, 6 }, { 64, 64 });

		FrameBufferSpecification spec;
		spec.width = 1280;
		spec.height = 720;
		m_FrameBuffer = FrameBuffer::create(spec);
	}

	void EditorLayer::onDetach()
	{
		GCE_PROFILE_FUNCTION();

	}

	void EditorLayer::onUpdate(Timestep ts)
	{
		GCE_PROFILE_FUNCTION();

		//Update
		m_CameraController.onUpdate(ts);

		//Render
		{
			GCE_PROFILE_SCOPE("Renderer Prep");
			Renderer2D::resetStats();
			m_FrameBuffer->bind();
			RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::clear();
		}

		{
			GCE_PROFILE_SCOPE("Renderer draw");

			Renderer2D::beginScene(m_CameraController.getCamera());

			Renderer2D::drawQuad(m_Position, m_Scale, m_Color);
			Renderer2D::drawQuad(m_Position + glm::vec2(0.2f, -0.5f), m_Scale, m_Color);
			Renderer2D::drawQuadRotated(m_Position + glm::vec2(-1.0f, -1.0f), m_Scale, m_Rotation, m_Color);
			Renderer2D::drawQuadRotated(m_Position + glm::vec2(0.7f, 0.7f), m_Scale, m_Rotation, texture, m_TextureScale, m_Color);
			Renderer2D::drawQuadRotated(m_Position + glm::vec2(1.3f, -1.3f), m_Scale, m_Rotation, stairs, m_Color);

			Renderer2D::endScene();
			m_FrameBuffer->unbind();
		}
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
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}


		if (ImGui::BeginMenuBar())
		{
			ImGui::Text("Menu Bar not added yet!");

			ImGui::EndMenuBar();
		}

		draw();

		ImGui::End();
	}

	void EditorLayer::onEvent(Event& e)
	{
		m_CameraController.onEvent(e);
	}


	void EditorLayer::draw()
	{
		ImGui::Begin("Settings");

		ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
		ImGui::DragFloat2("Position", glm::value_ptr(m_Position), m_DragSpeed);
		ImGui::DragFloat2("Scale", glm::value_ptr(m_Scale), m_DragSpeed);
		ImGui::SliderAngle("Rotation", &m_Rotation, -180.0f, 180.0f);
		ImGui::SliderInt("Texture scale", &m_TextureScale, 1, 10);
		ImGui::Spacing();

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

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewPortSize != *((glm::vec2*)&viewportPanelSize))
		{
			//GCE_TRACE("{0}, {1}", viewportPanelSize.x, viewportPanelSize.y);
			m_ViewPortSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_FrameBuffer->resize((unsigned int)m_ViewPortSize.x, (unsigned int)m_ViewPortSize.y);
			m_CameraController.onResize(m_ViewPortSize.x, m_ViewPortSize.y);
		}

		unsigned int textureID = m_FrameBuffer->getColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2(m_ViewPortSize.x, m_ViewPortSize.y), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
	}
}