#pragma once

#include "GCE/Input.h"

namespace GCE
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool isKeyPressedImpl(int keycode) override;
		virtual bool isMouseButtonPressedImpl(int button) override;

		virtual std::pair<float, float> getMousePositionImpl();
		virtual float getMouseXImpl() override;
		virtual float getMouseYImpl() override;
	};

}

