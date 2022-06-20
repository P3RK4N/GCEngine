#pragma once

#include "GCE/Layer.h"

namespace GCE
{
	class GCE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void onAttach();
		void onDetach();
		void onUpdate();
		void onEvent(Event& event);

	private:

	};
}