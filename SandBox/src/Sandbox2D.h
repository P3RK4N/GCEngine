#pragma once
#include "GCE.h"

class Sandbox2D : public GCE::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void onUpdate(GCE::Timestep ts) override;
	virtual void onImGuiRender() override;
	virtual void onEvent(GCE::Event& e) override;

	virtual void onAttach() override;
	virtual void onDetach() override;

public:
	void draw();

private:
	GCE::OrthographicCameraController m_CameraController;

	//TEMP
	GCE::Ref<GCE::Texture2D> texture;
	GCE::Ref<GCE::Texture2D> spriteSheet;
	GCE::Ref<GCE::SubTexture2D> stairs;

	glm::vec4 m_Color = glm::vec4(1.0f);
	glm::vec2 m_Position = glm::vec2(0.0f);
	glm::vec2 m_Scale = glm::vec2(1.0f);
	float m_Rotation = 0.0f;
	int m_TextureScale = 1;

	float m_DragSpeed = 0.1f;
};