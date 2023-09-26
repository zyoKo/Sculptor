#include <SculptorPch.h>

#include "WindowsWindow.h"

#include "GLFW/glfw3.h"

namespace Sculptor::Core
{
	WindowsWindow::WindowsWindow()
		:	window(nullptr)
	{
	}

	bool WindowsWindow::InitializeWindow(const WindowProperties& windowProperties)
	{
		const auto success = glfwInit();
		if (success != GLFW_TRUE)
		{
			std::cerr << "Failed to initialize GLFW!" << std::endl;
			__debugbreak();
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// TODO: Add Monitor Properties
		window = glfwCreateWindow(windowProperties.windowWidth, windowProperties.windowHeight, windowProperties.windowTitle, nullptr, nullptr);
		if (!window)
		{
			std::cerr << "Failed to create GLFW WindowsWindow!" << std::endl;
			__debugbreak();
		}

		return true;
	}

	GLFWwindow* WindowsWindow::GetGLFWWindow() const
	{
		return window;
	}

	bool WindowsWindow::WindowShouldClose() const
	{
		return glfwWindowShouldClose(window);
	}

	void WindowsWindow::Shutdown() const
	{
		glfwDestroyWindow(window);

		glfwTerminate();
	}

	void WindowsWindow::PollEvents() const
	{
		glfwPollEvents();
	}
}
