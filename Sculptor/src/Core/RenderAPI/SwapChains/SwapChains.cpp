#include <SculptorPch.h>

#include "SwapChains.h"

#include "Core/RenderAPI/Devices/PhysicalDevice.h"
#include "Core/Windows/VulkanWindowSurface/VulkanWindowSurface.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Utilities/Logger/Assert.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Core
{
	SwapChains::SwapChains()
		:	swapChain(nullptr)
	{ }

	void SwapChains::CreateSwapChain(const std::weak_ptr<Windows::VulkanWindowSurface>& windowSurface,
		const std::weak_ptr<LogicalDevice>& logicalDevice)
	{
		const auto device = logicalDevice.lock();
		S_ASSERT(device == nullptr, "Logical Device not set before creating SwapChain!");
		this->logicalDevice = logicalDevice;

		const auto window = windowSurface.lock();
		if (!window)
		{
			std::cerr << "Window Surface is not initialized while creating Swap Chain!" << std::endl;
			return;
		}

		const SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(windowSurface, device->GetPhysicalDevice());

		const auto surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
		const auto presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
		const auto extent = ChooseSwapExtent(swapChainSupport.capabilities, window->GetNativeWindow());

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
		createInfo.surface = window->GetSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		// specifies amount of layers each image consists of (unless making Stereoscopic 3D application, eg. VR)
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // Use 'VK_IMAGE_USAGE_TRANSFER_DST_BIT' for operations like Post Processing

		// Need to specify how to handle swap chain images that will be used across multiple queue families
		// Render to graphics queue family then transfer the image to presentation queue
		const auto indices = device->GetQueueFamilies().GetQueueFamilyIndices();
		const std::array<uint32_t, 2> queueFamilyIndices = {
			indices.graphicsFamily.value(),
			indices.presetFamily.value()
		};

		// VK_SHARING_MODE_CONCURRENT: Images can be used across multiple queue families without explicit ownership transfers
		// VK_SHARING_MODE_EXCLUSIVE: An image is owned by one queue family at a time and ownership must be explicitly transferred
		// before using it in another queue family. This option offers the best performance.
		if (indices.graphicsFamily != indices.presetFamily)
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
		// so we need to recreate swap chain (later)
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		const auto result = vkCreateSwapchainKHR(device->GetLogicalDevice(), &createInfo, nullptr, &swapChain);
		S_ASSERT(result != VK_SUCCESS, "Failed to create Swapchain!");
	}

	void SwapChains::SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device)
	{
		this->logicalDevice = device;
	}

	void SwapChains::CleanUp() const
	{
		const auto device = logicalDevice.lock();
		S_ASSERT(device == nullptr, "Logical Device is not set.");

		vkDestroySwapchainKHR(device->GetLogicalDevice(), swapChain, nullptr);
	}

	SwapChainSupportDetails SwapChains::QuerySwapChainSupport(const std::weak_ptr<Windows::VulkanWindowSurface>& windowSurface,
		const std::weak_ptr<PhysicalDevice>& physicalDevice)
	{
		SwapChainSupportDetails details;

		const auto surfacePtr = windowSurface.lock();
		S_ASSERT(surfacePtr == nullptr, "Initialize Window Surface before querying for Swap Chain support!");
		const auto& surface = surfacePtr->GetSurface();

		const auto physicalDevicePtr = physicalDevice.lock();
		S_ASSERT(physicalDevicePtr == nullptr, "Physical Device is null!");
		const auto& device = physicalDevicePtr->GetPrimaryPhysicalDevice();

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		// Check for supported surface formats
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		// Check for supported presentation modes
		uint32_t presetModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presetModeCount, nullptr);

		if (presetModeCount != 0)
		{
			details.presentModes.resize(presetModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presetModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR SwapChains::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
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

	VkPresentModeKHR SwapChains::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
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

	VkExtent2D SwapChains::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const std::weak_ptr<WindowsWindow>& window)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		
		const auto windowPtr = window.lock();
		if (!windowPtr)
		{
			// TODO handle this
		}

		int width, height;
		glfwGetFramebufferSize(windowPtr->GetGLFWWindow(), &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}
