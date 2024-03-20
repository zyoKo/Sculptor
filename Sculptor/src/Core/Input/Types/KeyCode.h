#pragma once

namespace Sculptor::Core
{
	enum class KeyCode
	{
		KEY_W,
		KEY_A,
		KEY_S,
		KEY_D,
		KEY_NONE,
		TOTAL_KEYS
	};

	KeyCode GlfwKeyToEngineKeyCode(int glfwKey);
}
