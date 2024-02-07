#pragma once

#include "SwapChainSupportDetails.h"
#include "Utilities/Macros.h"

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

		void Create(const std::weak_ptr<Windows::VulkanWindowSurface>& windowSurface,
			const std::weak_ptr<LogicalDevice>& logicalDevice);

		const VkSwapchainKHR& Get() const;

		const VkExtent2D& GetSwapChainExtent() const;

		void CleanUp() const;

		LOGICAL_DEVICE

	private:
		VkSwapchainKHR swapChain;

		std::vector<VkImage> swapChainImages;

		VkFormat swapChainImageFormat;

		VkExtent2D swapChainExtent;

		friend class LogicalDevice;

		friend class RenderApi;

		friend class SwapChainImageView;

		friend class GraphicsPipeline;

		friend class FrameBuffer;

		friend class CommandBuffer;

		static SwapChainSupportDetails QuerySwapChainSupport(const std::weak_ptr<Windows::VulkanWindowSurface>& windowSurface,
			const std::weak_ptr<PhysicalDevice>& physicalDevice);

		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const std::weak_ptr<WindowsWindow>& window);
	};
}
