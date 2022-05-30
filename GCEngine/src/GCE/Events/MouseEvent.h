#pragma once

#include "Event.h"

namespace GCE
{
	class GCE_API MouseMovedEvent : public Event
	{
	private:
		float m_MouseX;
		float m_MouseY;

	public:
		MouseMovedEvent(float x, float y)
		{
			m_MouseX = x;
			m_MouseY = y;
		}

		inline float getX() const { return m_MouseX; }
		inline float getY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	};

	class GCE_API MouseScrolledEvent : public Event
	{
	private:
		float m_XOffset;
		float m_YOffset;

	public:
		MouseScrolledEvent(float xOffset, float yOffset)
		{
			m_XOffset = xOffset;
			m_YOffset = yOffset;
		}

		inline float getXOffset() const { return m_XOffset; }
		inline float getYOffset() const { return m_YOffset; }

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	};

	class GCE_API MouseButtonEvent : public Event
	{
	public:
		inline int getMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		int m_Button;
		MouseButtonEvent(int button)
		{
			m_Button = button;
		}	
	};

	class GCE_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}
		
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class GCE_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}
