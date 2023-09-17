#pragma once

#include "WindowConstants.h"

namespace Sculptor::Core
{
	struct WindowProperties
	{
		int windowWidth;

		int windowHeight;

		const char* windowTitle;

		constexpr WindowProperties()
			:	windowWidth(WINDOW_WIDTH),
				windowHeight(WINDOW_HEIGHT),
				windowTitle(WINDOW_TITLE)
		{
		}

		constexpr WindowProperties(int windowWidth, int windowHeight, const char* windowTitle = WINDOW_TITLE)
			:	windowWidth(windowWidth),
				windowHeight(windowHeight),
				windowTitle(windowTitle)
		{
		}
	};
}
