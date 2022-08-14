#include "GCE.h"

#include "EditorLayer.h"

namespace GCE
{
	class GCEditor : public Application
	{
	public:
		GCEditor(ApplicationCommandLineArgs args) :
			Application("GCEditor", args)
		{
			pushLayer(new EditorLayer());
		}

		~GCEditor()
		{

		}
	};

	Application* createApplication(ApplicationCommandLineArgs args)
	{
		return new GCEditor(args);
	}
}
