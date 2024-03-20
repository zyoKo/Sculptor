#include <SculptorPch.h>

#include <GLFW/glfw3.h>

#include "InputAction.h"

namespace Sculptor::Core
{
	InputAction GlfwInputActionToEngineInputAction(int glfwAction)
	{
		InputAction action;

		switch(glfwAction)
		{
		case GLFW_PRESS:
			action = InputAction::PRESSED;
			break;

		case GLFW_RELEASE:
			action = InputAction::RELEASED;
			break;

		case GLFW_REPEAT:
			action = InputAction::REPEAT;
			break;

		default:
			action = InputAction::NONE;
			break;
		}

		return action;
	}
}
