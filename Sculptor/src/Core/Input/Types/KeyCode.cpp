#include <SculptorPch.h>

#include <GLFW/glfw3.h>

#include "KeyCode.h"

namespace Sculptor::Core
{
	KeyCode GlfwKeyToEngineKeyCode(int glfwKey)
	{
		KeyCode keyCode;

		switch (glfwKey)
		{
		case GLFW_KEY_W:
			keyCode = KeyCode::KEY_W;
			break;

		case GLFW_KEY_A:
			keyCode = KeyCode::KEY_A;
			break;

		case GLFW_KEY_S:
			keyCode = KeyCode::KEY_S;
			break;

		case GLFW_KEY_D:
			keyCode = KeyCode::KEY_D;
			break;

		default: 
			keyCode = KeyCode::KEY_NONE;
			break;
		}

		return keyCode;
	}
}
