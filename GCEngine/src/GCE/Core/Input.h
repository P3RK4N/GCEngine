#pragma once

#include "GCE/Core/Core.h"

namespace GCE
{
	class GCE_API Input
	{
	public:
		static bool isKeyPressed(int keycode);
		static bool isMouseButtonPressed(int button);

		static std::pair<float, float> getMousePosition();
		static float getMouseX();
		static float getMouseY();
	};
}