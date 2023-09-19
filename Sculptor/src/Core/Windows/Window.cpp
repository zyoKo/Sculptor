#include <SculptorPch.h>

#include "Window.h"

#include "GLFW/glfw3.h"

namespace Sculptor::Core
{
	Window::Window()
		:	window(nullptr)
	{
	}

	bool Window::InitializeWindow(const WindowProperties& windowProperties)
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
			std::cerr << "Failed to create GLFW Window!" << std::endl;
			__debugbreak();
		}

		return true;
	}

	GLFWwindow* Window::GetWindow() const
	{
		return window;
	}

	bool Window::WindowShouldClose() const
	{
		return glfwWindowShouldClose(window);
	}

	void Window::Shutdown() const
	{
		glfwDestroyWindow(window);

		glfwTerminate();
	}

	void Window::PollEvents() const
	{
		glfwPollEvents();
	}
}
