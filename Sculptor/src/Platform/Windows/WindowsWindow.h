#pragma once

#include <GLFW/glfw3.h>

#include "WindowData/WindowProperties.h"

namespace Sculptor::Core
{
	class WindowsWindow
	{
	public:
		WindowsWindow() noexcept;

		explicit WindowsWindow(WindowsWindow&) = delete;

		explicit WindowsWindow(WindowsWindow&&) = delete;

		WindowsWindow& operator=(WindowsWindow&) = delete;

		WindowsWindow& operator=(WindowsWindow&&) = delete;

		~WindowsWindow() = default;

		bool InitializeWindow(const WindowProperties& windowProperties);

		GLFWwindow* GetGLFWWindow() const;

		bool WindowShouldClose() const;

		void Shutdown() const;

		void PollEvents() const;

	private:
		GLFWwindow* window;

		bool frameBufferResized;

		static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	};
}
