#include <SculptorPch.h>

#include "WindowsWindow.h"

#include "Core/Input/Input.h"
#include "GLFW/glfw3.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	WindowsWindow::WindowsWindow() noexcept
		:	window(nullptr),
			windowProperties(nullptr),
			frameBufferResized(false)
	{ }

	bool WindowsWindow::InitializeWindow(std::unique_ptr<WindowProperties> properties)
	{
		this->windowProperties = std::move(properties);

		const auto success = glfwInit();
		S_ASSERT(success != GLFW_TRUE, "Failed to initialize GLFW!")

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(windowProperties->windowWidth, windowProperties->windowHeight, windowProperties->windowTitle, nullptr, nullptr);
		S_ASSERT(window == nullptr, "Failed to create GLFW window.")

		Input::Initialize();

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
		glfwSetKeyCallback(window, KeyboardKeyCallback);
		glfwSetMouseButtonCallback(window, MouseKeyCallback);
		glfwSetCursorPosCallback(window, MouseCursorCallback);

		return true;
	}

	const WindowProperties* WindowsWindow::GetWindowProperties() const
	{
		return windowProperties.get();
	}

	GLFWwindow* WindowsWindow::GetGLFWWindow() const
	{
		return window;
	}

	bool WindowsWindow::WindowShouldClose() const
	{
		int result = glfwWindowShouldClose(window);

		if (!result)
		{
			this->UpdateCursorVisibility();
		}

		return result;
	}

	void WindowsWindow::Shutdown() const
	{
		glfwDestroyWindow(window);

		glfwTerminate();
	}

	void WindowsWindow::PollEvents()
	{
		glfwPollEvents();
	}

	void WindowsWindow::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		const auto app = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		app->frameBufferResized = true;
	}

	void WindowsWindow::KeyboardKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
	{
		Input::KeyCallback(key, scanCode, action, mods);
	}

	void WindowsWindow::MouseKeyCallback(GLFWwindow* window, int button, int action, int mods)
	{
	}

	void WindowsWindow::MouseCursorCallback(GLFWwindow* window, double xPosition, double yPosition)
	{
		Input::MousePositionCallback(xPosition, yPosition);
	}

	void WindowsWindow::UpdateCursorVisibility() const
	{
		if (!Input::isCursorVisibilityDirty)
		{
			return;
		}

		if (Input::isCursorVisible)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			Input::isCursorVisibilityDirty = false;
			return;
		}

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
