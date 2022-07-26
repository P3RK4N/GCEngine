#include "Sandbox2D.h"

#include <GCE/Core/EntryPoint.h>

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() :
	Layer("Sandbox2D"),
	m_CameraController(16.0f / 9.0f),
	texture(GCE::Texture2D::create("assets/textures/flowerPattern.jpg")),
	spriteSheet(GCE::Texture2D::create("assets/textures/spriteSheet.png"))
{

}

void Sandbox2D::onAttach()
{
	GCE_PROFILE_FUNCTION();

	stairs = GCE::SubTexture2D::createFromCoords(spriteSheet, { 7, 6 }, { 64, 64 });
}

void Sandbox2D::onDetach()
{
	GCE_PROFILE_FUNCTION();

}

void Sandbox2D::onUpdate(GCE::Timestep ts)
{
	GCE_PROFILE_FUNCTION();

	//Update
	m_CameraController.onUpdate(ts);

	//Render
	{
		GCE_PROFILE_SCOPE("Renderer Prep");
		GCE::Renderer2D::resetStats();
		GCE::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		GCE::RenderCommand::clear();
	}

	{
		GCE_PROFILE_SCOPE("Renderer draw");

		GCE::Renderer2D::beginScene(m_CameraController.getCamera());

		GCE::Renderer2D::drawQuad(m_Position, m_Scale, m_Color);
		GCE::Renderer2D::drawQuad(m_Position + glm::vec2(0.2f, -0.5f), m_Scale, m_Color);
		GCE::Renderer2D::drawQuadRotated(m_Position + glm::vec2(-1.0f, -1.0f), m_Scale, m_Rotation, m_Color);
		GCE::Renderer2D::drawQuadRotated(m_Position + glm::vec2(0.7f, 0.7f), m_Scale, m_Rotation, texture, m_TextureScale, m_Color);
		GCE::Renderer2D::drawQuadRotated(m_Position + glm::vec2(1.3f, -1.3f), m_Scale, m_Rotation, stairs, m_Color);

		GCE::Renderer2D::endScene();
	}
}

void Sandbox2D::onImGuiRender()
{
	GCE_PROFILE_FUNCTION();

	draw();
}

void Sandbox2D::onEvent(GCE::Event& e)
{
	m_CameraController.onEvent(e);
}


void Sandbox2D::draw()
{
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
	ImGui::DragFloat2("Position", glm::value_ptr(m_Position), m_DragSpeed);
	ImGui::DragFloat2("Scale", glm::value_ptr(m_Scale), m_DragSpeed);
	ImGui::SliderAngle("Rotation", &m_Rotation, -180.0f, 180.0f);
	ImGui::SliderInt("Texture scale", &m_TextureScale, 1, 10);
	ImGui::Spacing();

	auto stats = GCE::Renderer2D::getStats();
	ImGui::Text("Renderer2D stats:");
	ImGui::Text("Draw calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());

	ImGui::End();
}