#pragma once

#include "GCE/Layer.h"

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

		void onAttach();
		void onDetach();
		void onUpdate();
		void onEvent(Event& event);

	private:
		float m_Time = 0.0f;

		bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool onMouseScrolledEvent(MouseScrolledEvent& e);
		bool onMouseMovedEvent(MouseMovedEvent& e);
		bool onKeyPressedEvent(KeyPressedEvent& e);
		bool onKeyReleasedEvent(KeyReleasedEvent& e);
		bool onKeyTypedEvent(KeyTypedEvent& e);
		bool onWindowResizeEvent(WindowResizeEvent& e);
	};
}