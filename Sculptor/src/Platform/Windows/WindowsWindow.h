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

		bool InitializeWindow(std::unique_ptr<WindowProperties> windowProperties);

		const WindowProperties* GetWindowProperties() const;

		GLFWwindow* GetGLFWWindow() const;

		bool WindowShouldClose() const;

		void Shutdown() const;

		static void PollEvents();

	private:
		GLFWwindow* window;

		std::unique_ptr<WindowProperties> windowProperties;

		bool frameBufferResized;

		static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

		static void CursorPositionCallback(GLFWwindow* window, double xOffset, double yOffset);

		static void KeyboardKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);

		static void MouseKeyCallback(GLFWwindow* window, int button, int action, int mods);
	};
}
