#include <SculptorPch.h>

#include <GLFW/glfw3.h>

#include "VulkanWindowSurface.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Core/RenderAPI/VulkanInstance/VulkanInstanceWrapper.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Windows
{
	VulkanWindowSurface::VulkanWindowSurface()
		:	windowSurface(nullptr)
	{ }

	void VulkanWindowSurface::Create(const std::shared_ptr<Core::VulkanInstanceWrapper>& vulkanInstanceWrapper,
		const std::shared_ptr<Core::WindowsWindow>& nativeWindow)
	{
		const auto success = glfwCreateWindowSurface(vulkanInstanceWrapper->GetInstance(), nativeWindow->GetGLFWWindow(), nullptr, &windowSurface);
		S_ASSERT(success != VK_SUCCESS, "Failed to create window surface!")

		this->nativeWindow = nativeWindow;
	}

	void VulkanWindowSurface::CleanUp(const std::shared_ptr<Core::VulkanInstanceWrapper>& vulkanInstanceWrapper) const
	{
		vkDestroySurfaceKHR(vulkanInstanceWrapper->GetInstance(), windowSurface, nullptr);
	}

	const VkSurfaceKHR& VulkanWindowSurface::GetSurface() const
	{
		return windowSurface;
	}

	VkSurfaceKHR& VulkanWindowSurface::GetSurface()
	{
		return windowSurface;
	}

	std::weak_ptr<Core::WindowsWindow> VulkanWindowSurface::GetNativeWindow() const
	{
		return nativeWindow;
	}
}
