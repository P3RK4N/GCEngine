#include "GCE.h"

class SandBox : public GCE::Application
{
public:
	SandBox()
	{
		
	}

	~SandBox()
	{

	}
};

GCE::Application* GCE::createApplication()
{
	return new SandBox();
}
