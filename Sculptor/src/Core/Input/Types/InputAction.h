#pragma once

namespace Sculptor::Core
{
	enum class InputAction
	{
		PRESSED,
		RELEASED,
		REPEAT,
		HELD,
		NONE,
		TOTAL_ACTIONS
	};

	InputAction GlfwInputActionToEngineInputAction(int glfwAction);
}
