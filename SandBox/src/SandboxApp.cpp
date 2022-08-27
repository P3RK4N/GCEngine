#include "GCE.h"

#include "Sandbox2D.h"


class SandBox : public GCE::Application
{
public:
	SandBox(const ApplicationSpecification& specification)
		: Application(specification)
	{
		//pushLayer(new ExampleLayer());
		pushLayer(new Sandbox2D());
	}

	~SandBox()
	{

	}
};

GCE::Application* GCE::createApplication(GCE::ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.name = "Sandbox";
	spec.workingDirectory = "../GCEditor";
	spec.commandLineArgs = args;

	return new SandBox(spec);
}
