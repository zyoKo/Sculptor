#pragma once

#include "SwapChainSupportDetails.h"

namespace Sculptor::Windows
{
	class VulkanWindowSurface;
}

namespace Sculptor::Core
{
	class LogicalDevice;

	class PhysicalDevice;

	class WindowsWindow;
}

namespace Sculptor::Core
{
	class SwapChain
	{
	public:
		SwapChain();

		~SwapChain() = default;

		void CreateSwapChain(const std::weak_ptr<Windows::VulkanWindowSurface>& windowSurface,
			const std::weak_ptr<LogicalDevice>& logicalDevice);

		void SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device);

		void CleanUp() const;

	private:
		VkSwapchainKHR swapChain;

		std::vector<VkImage> swapChainImages;

		VkFormat swapChainImageFormat;

		VkExtent2D swapChainExtent;

		std::weak_ptr<LogicalDevice> logicalDevice;

		friend class LogicalDevice;

		friend class RenderApi;

		friend class ImageViews;

		static SwapChainSupportDetails QuerySwapChainSupport(const std::weak_ptr<Windows::VulkanWindowSurface>& windowSurface,
			const std::weak_ptr<PhysicalDevice>& physicalDevice);

		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const std::weak_ptr<WindowsWindow>& window);
	};
}