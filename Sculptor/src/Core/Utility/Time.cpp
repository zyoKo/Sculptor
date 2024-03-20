#include <SculptorPch.h>

#include "Time.h"

#include "GLFW/glfw3.h"

namespace Sculptor::Core
{
	void Time::Update()
	{
		const auto currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	float Time::GetDeltaTime()
	{
		return deltaTime;
	}
}
