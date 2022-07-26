#include "GCE.h"

#include "Sandbox2D.h"


class SandBox : public GCE::Application
{
public:
	SandBox()
	{
		//pushLayer(new ExampleLayer());
		pushLayer(new Sandbox2D());
	}

	~SandBox()
	{

	}
};

GCE::Application* GCE::createApplication()
{
	return new SandBox();
}
