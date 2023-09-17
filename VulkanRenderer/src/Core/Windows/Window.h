#pragma once

#include "GLFW/glfw3.h"

#include "WindowData/WindowProperties.h"

namespace Sculptor::Core
{
	class Window
	{
	public:
		Window();

		Window(const Window&) = delete;

		Window& operator=(const Window&) = delete;

		~Window() = default;

		bool InitializeWindow(const WindowProperties& windowProperties);

		GLFWwindow* GetWindow() const;

		bool WindowShouldClose() const;

		void Shutdown() const;

		void PollEvents() const;

	private:
		GLFWwindow* window;
	};
}
