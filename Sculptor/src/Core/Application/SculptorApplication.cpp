#include <SculptorPch.h>

#include "SculptorApplication.h"

namespace Sculptor::Core
{
	SculptorApplication::SculptorApplication()
		:	window(nullptr),
			windowProperties(1920, 1080)
	{
		window = std::make_unique<Window>();
	}

	void SculptorApplication::RunApplication()
	{
		InitializeWindow();

		InitializeVulkan();

		MainLoop();

		CleanUp();
	}

	void SculptorApplication::InitializeWindow() const
	{
		window->InitializeWindow(windowProperties);
	}

	void SculptorApplication::InitializeVulkan()
	{
	}

	void SculptorApplication::MainLoop() const
	{
		while(!window->WindowShouldClose())
		{
			window->PollEvents();
		}
	}

	void SculptorApplication::CleanUp()
	{
		window->Shutdown();
	}
}
