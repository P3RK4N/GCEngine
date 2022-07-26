#include "GCE.h"

#include "EditorLayer.h"

namespace GCE
{
	class GCEditor : public Application
	{
	public:
		GCEditor() :
			Application("GCEditor")
		{
			pushLayer(new EditorLayer());
		}

		~GCEditor()
		{

		}
	};

	Application* createApplication()
	{
		return new GCEditor();
	}
}
