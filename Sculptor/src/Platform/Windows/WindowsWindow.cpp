#include <SculptorPch.h>

#include "WindowsWindow.h"

#include "GLFW/glfw3.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	WindowsWindow::WindowsWindow()
		:	window(nullptr),
			frameBufferResized(false)
	{ }

	bool WindowsWindow::InitializeWindow(const WindowProperties& windowProperties)
	{
		const auto success = glfwInit();
		if (success != GLFW_TRUE)
		{
			std::cerr << "Failed to initialize GLFW!" << std::endl;
			__debugbreak();
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		// TODO: Add Monitor Properties
		window = glfwCreateWindow(windowProperties.windowWidth, windowProperties.windowHeight, windowProperties.windowTitle, nullptr, nullptr);
		S_ASSERT(window == nullptr, "Failed to create GLFW window.");

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

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

	void WindowsWindow::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		const auto app = reinterpret_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		app->frameBufferResized = true;
	}
}
