#include "GCE.h"

#include "EditorLayer.h"

namespace GCE
{
	class GCEditor : public Application
	{
	public:
		GCEditor(const ApplicationSpecification& specification)
			: Application(specification)
		{
			pushLayer(new EditorLayer());
		}

		~GCEditor()
		{

		}
	};

	Application* createApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.name = "GCEditor";
		spec.commandLineArgs = args;

		return new GCEditor(spec);
	}
}
