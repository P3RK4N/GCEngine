#include "GCEPCH.h"
#include "GCE/Core/Input.h" 

#include "GCE/Core/Application.h"

#include <GLFW/glfw3.h>

namespace GCE
{
	bool Input::isKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::get()->getWindow().getNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::isMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::get()->getWindow().getNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::getMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::get()->getWindow().getNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	float Input::getMouseX()
	{
		auto [x, y] = Input::getMousePosition();
		return x;
	}

	float Input::getMouseY()
	{
		auto [x, y] = Input::getMousePosition();
		return y;
	}
}