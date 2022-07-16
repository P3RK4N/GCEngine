#pragma once

#include "GCE/Core/Layer.h"

#include "GCE/Events/KeyEvent.h"
#include "GCE/Events/MouseEvent.h"
#include "GCE/Events/ApplicationEvent.h"

namespace GCE
{
	class GCE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}