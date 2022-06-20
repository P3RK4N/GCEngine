#include "GCE.h"

class ExampleLayer : public GCE::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void onUpdate() override
	{
		GCE_INFO("ExampleLayer::Update");
	}

	void onEvent(GCE::Event& event) override
	{
		GCE_TRACE("{0}", event);
	}
};

class SandBox : public GCE::Application
{
public:
	SandBox()
	{
		pushLayer(new ExampleLayer());
		pushOverlay(new GCE::ImGuiLayer());
	}

	~SandBox()
	{

	}
};

GCE::Application* GCE::createApplication()
{
	return new SandBox();
}
