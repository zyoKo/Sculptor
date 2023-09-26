#pragma once

#include "GLFW/glfw3.h"

#include "WindowData/WindowProperties.h"

namespace Sculptor::Core
{
	class WindowsWindow
	{
	public:
		WindowsWindow();

		WindowsWindow(const WindowsWindow&) = delete;

		WindowsWindow& operator=(const WindowsWindow&) = delete;

		~WindowsWindow() = default;

		bool InitializeWindow(const WindowProperties& windowProperties);

		GLFWwindow* GetGLFWWindow() const;

		bool WindowShouldClose() const;

		void Shutdown() const;

		void PollEvents() const;

	private:
		GLFWwindow* window;
	};
}
