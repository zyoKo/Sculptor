#include <SculptorPch.h>

#include <GLFW/glfw3.h>

#include "VulkanWindowSurface.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Core/RenderAPI/VulkanInstance/VulkanInstanceWrapper.h"
#include "Core/RenderAPI/ValidationLayer/ValidationLayer.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Windows
{
	VulkanWindowSurface::VulkanWindowSurface()
		:	windowSurface(nullptr)
	{
	}

	void VulkanWindowSurface::CreateWindowSurface()
	{
		const auto success = glfwCreateWindowSurface(vulkanInstanceWrapper->GetInstance(), nativeWindow->GetGLFWWindow(), nullptr, &windowSurface);
		S_ASSERT(success != VK_SUCCESS, "Failed to create window surface!");

		// NOTE: These steps can be skipped when creating WindowSurface when not using Windowing Library like GLFW
		//VkWin32SurfaceCreateInfoKHR createSurfaceInfo{};
		//createSurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		//createSurfaceInfo.hwnd = glfwGetWin32Window(nativeWindow->GetGLFWWindow());	// glfwGetWin32Window gets the HWND Module(Microsoft Windows APIs HWND)
		//createSurfaceInfo.hinstance = GetModuleHandle(nullptr);	// GetModuleHandle returns the HINSTANCE handle of the current process

		//auto success = vkCreateWin32SurfaceKHR(vulkanInstanceWrapper->GetInstance(), &createSurfaceInfo, nullptr, &windowSurface);
		//S_ASSERT(success != VK_SUCCESS, "Failed to create window surface!");
	}

	void VulkanWindowSurface::CleanUp() const
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

	void VulkanWindowSurface::SetNativeWindow(const std::shared_ptr<Core::WindowsWindow>& nativeWindow)
	{
		this->nativeWindow = nativeWindow;
	}

	const std::shared_ptr<Core::WindowsWindow>& VulkanWindowSurface::GetNativeWindow() const
	{
		return nativeWindow;
	}

	void VulkanWindowSurface::SetVulkanInstanceWrapper(const std::shared_ptr<Core::VulkanInstanceWrapper>& vulkanInstanceWrap)
	{
		this->vulkanInstanceWrapper = vulkanInstanceWrap;
	}

	const std::shared_ptr<Core::VulkanInstanceWrapper>& VulkanWindowSurface::GetVulkanInstanceWrapper() const
	{
		return vulkanInstanceWrapper;
	}

	void VulkanWindowSurface::SetValidationLayer(const std::shared_ptr<Core::ValidationLayer>& validationLayer)
	{
		this->validationLayer = validationLayer;
	}

	const std::shared_ptr<Core::ValidationLayer>& VulkanWindowSurface::GetValidationLayer() const
	{
		return validationLayer;
	}
}
