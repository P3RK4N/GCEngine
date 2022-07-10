#include "GCE.h"

class ExampleLayer : public GCE::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void onEvent(GCE::Event& event) override
	{

	}
};

class SandBox : public GCE::Application
{
public:
	SandBox()
	{
		pushLayer(new ExampleLayer());
	}

	~SandBox()
	{

	}
};

GCE::Application* GCE::createApplication()
{
	return new SandBox();
}
