#include <SculptorPch.h>

#include "SwapChain.h"

#include "Core/RenderAPI/Devices/PhysicalDevice.h"
#include "Core/Windows/VulkanWindowSurface/VulkanWindowSurface.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Core
{
	SwapChain::SwapChain(std::weak_ptr<LogicalDevice> logicalDevicePtr) noexcept
		:	logicalDevice(std::move(logicalDevicePtr)),
			swapChain(nullptr),
			swapChainImageFormat{},
			swapChainExtent{}
	{ }

	void SwapChain::Create(const std::weak_ptr<Windows::VulkanWindowSurface>& windowSurface)
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();
		const auto& physicalDevicePtr = logicalDevicePtr->GetPhysicalDevice();

		GetShared<Windows::VulkanWindowSurface> windowSurfacePtr{ windowSurface };

		const SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(windowSurface, physicalDevicePtr);

		const auto surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
		swapChainImageFormat = surfaceFormat.format;

		const auto presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);

		const auto extent = ChooseSwapExtent(swapChainSupport.capabilities, windowSurfacePtr->GetNativeWindow());
		swapChainExtent = extent;

		// We do +1 cause sometimes we have to wait on the driver to complete internal operations
		// before we can acquire another image to render to
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

		// check to make sure we don't exceed the maximum number of images
		// '0' is a special value that means that there is no maximum
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = windowSurfacePtr->GetSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		// specifies amount of layers each image consists of (unless making Stereoscopic 3D application, eg. VR)
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // Use 'VK_IMAGE_USAGE_TRANSFER_DST_BIT' for operations like Post Processing

		// Need to specify how to handle swap chain images that will be used across multiple queue families
		// Render to graphics queue family then transfer the image to presentation queue
		const auto indices = logicalDevicePtr->GetQueueFamilies().GetQueueFamilyIndices();
		const std::array<U32, 2> queueFamilyIndices = {
			indices.GetGraphicsFamily().value(),
			indices.GetPresetFamily().value()
		};

		// VK_SHARING_MODE_CONCURRENT: Images can be used across multiple queue families without explicit ownership transfers
		// VK_SHARING_MODE_EXCLUSIVE: An image is owned by one queue family at a time and ownership must be explicitly transferred
		// before using it in another queue family. This option offers the best performance.
		if (indices.GetGraphicsFamily().value() != indices.GetPresetFamily().value())
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		// if certain transforms need to be applied to the image else use the current transform
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

		// specifies if the alpha channel should be used for blending with other windows
		// we always want to ignore the alpha channel
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;	// VK_TRUE means we don't care about the color of the pixels that are obscured

		// while application is running the swap chain might become invalid or unoptimized
		// so, we need to recreate swap chain (later)
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		VK_CHECK(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain), "Failed to create Swapchain!")

		VK_CHECK(vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr), "Failed to get swapChainImages")
		swapChainImages.resize(imageCount);
		VK_CHECK(vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data()), 
			"Failed to get swapChainImages after resize")
	}

	const VkSwapchainKHR& SwapChain::Get() const
	{
		return swapChain;
	}

	const VkExtent2D& SwapChain::GetSwapChainExtent() const
	{
		return swapChainExtent;
	}

	void SwapChain::CleanUp() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		vkDestroySwapchainKHR(device, swapChain, nullptr);
	}

	SwapChainSupportDetails SwapChain::QuerySwapChainSupport(const std::weak_ptr<Windows::VulkanWindowSurface>& windowSurface,
		const std::weak_ptr<PhysicalDevice>& weakPhysicalDevice)
	{
		SwapChainSupportDetails details;

		GetShared<Windows::VulkanWindowSurface> surfacePtr{ windowSurface };
		const auto& surface = surfacePtr->GetSurface();

		GetShared<PhysicalDevice> physicalDevicePtr { weakPhysicalDevice };
		const auto& physicalDevice = physicalDevicePtr->GetPrimaryDevice();

		VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities), 
			"Failed to get Physcial Device Surface Capabilities.")

		// Check for supported surface formats
		uint32_t formatCount;
		VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr),
			"Failed to get Physical Device Surface Format.")

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
		}

		// Check for supported presentation modes
		uint32_t presetModeCount;
		VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presetModeCount, nullptr),
			"Failed to get Physical Device Surface Present Mode KHR.")

		if (presetModeCount != 0)
		{
			details.presentModes.resize(presetModeCount);
			VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presetModeCount, details.presentModes.data()),
				"Failed to get Physical Device Surface Present Mode KHR.")
		}

		return details;
	}

	VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const std::weak_ptr<WindowsWindow>& window)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}

		GetShared<WindowsWindow> windowPtr{ window };

		int width, height;
		glfwGetFramebufferSize(windowPtr->GetGLFWWindow(), &width, &height);

		VkExtent2D actualExtent = { static_cast<U32>(width), static_cast<U32>(height) };

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}
