#include <SculptorPch.h>

#include <functional>

#include <GLFW/glfw3.h>

#include "KeyCode.h"

namespace Sculptor::Core
{
	KeyCode GlfwKeyToEngineKeyCode(int glfwKey)
	{
		// TODO: Remove this before merging
		//std::cout << "GLFW KEY: " << glfwKey << "\n";

		// A to Z
		if (glfwKey >= GLFW_KEY_A && glfwKey <= GLFW_KEY_Z)
		{
			return static_cast<KeyCode>(glfwKey - GLFW_KEY_A + static_cast<int>(KeyCode::KEY_A));
		}

		// 0 to 9
		if (glfwKey >= GLFW_KEY_0 && glfwKey <= GLFW_KEY_9)
		{
			return static_cast<KeyCode>(glfwKey - GLFW_KEY_0 + static_cast<int>(KeyCode::KEY_0));
		}

		// F1 to F12
		if (glfwKey >= GLFW_KEY_F1 && glfwKey <= GLFW_KEY_F9)
		{
			return static_cast<KeyCode>(glfwKey - GLFW_KEY_F9 + static_cast<int>(KeyCode::KEY_F9));
		}

		// All LEFT to RIGHT Keys
		if (glfwKey >= GLFW_KEY_LEFT_SHIFT && glfwKey <= GLFW_KEY_RIGHT_SUPER)
		{
			return static_cast<KeyCode>(glfwKey - GLFW_KEY_LEFT_SHIFT + static_cast<int>(KeyCode::KEY_LEFT_SHIFT));
		}

#pragma region SPECIAL_KEYS
		if (glfwKey >= GLFW_KEY_ESCAPE && glfwKey <= GLFW_KEY_PAUSE)
		{
			return static_cast<KeyCode>(glfwKey - GLFW_KEY_ESCAPE + static_cast<int>(KeyCode::KEY_ESCAPE));
		}

		if (glfwKey >= GLFW_KEY_SPACE && glfwKey <= GLFW_KEY_SLASH)
		{
			return static_cast<KeyCode>(glfwKey - GLFW_KEY_SPACE + static_cast<int>(KeyCode::KEY_SPACE));
		}

		switch (glfwKey)
		{
		case GLFW_KEY_SEMICOLON:
			return KeyCode::KEY_SEMICOLON;

		case GLFW_KEY_EQUAL:
			return KeyCode::KEY_EQUAL;

		default: 
			return KeyCode::KEY_NONE;
		}
#pragma endregion
	}

	void PrintKeyUsed(KeyCode keyCode)
	{
		std::cout << "Key Used: " << static_cast<char>(keyCode) << '\n';
	}
}
