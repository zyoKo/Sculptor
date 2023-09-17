#pragma once

#include "Core/Windows/Window.h"

namespace Sculptor::Core
{
	class SculptorApplication
	{
	public:
		SculptorApplication();

		void RunApplication();

	private:
		std::unique_ptr<Window> window;

		WindowProperties windowProperties;

		void InitializeWindow() const;

		void InitializeVulkan();

		void MainLoop() const;

		void CleanUp();
	};
}
