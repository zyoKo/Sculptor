#include <SculptorPch.h>

#include "DepthTesting.h"

namespace Sculptor::Core
{
	DepthTesting::DepthTesting(std::weak_ptr<LogicalDevice> device, std::weak_ptr<SwapChain> swapChain) noexcept
		:	VulkanTexture(),
			ImageView(),
			logicalDevice(std::move(device)),
			swapChain(std::move(swapChain)),
			depthImageView(VK_NULL_HANDLE)
	{
		VulkanTexture::SetLogicalDevice(this->logicalDevice);
	}

	void DepthTesting::Create()
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto device = logicalDevicePtr->Get();

		GetShared<PhysicalDevice> physicalDevicePtr{ logicalDevicePtr->GetPhysicalDevice() };
		const auto physicalDevice = physicalDevicePtr->GetPrimaryDevice();

		GetShared<SwapChain> swapChainPtr{ swapChain };
		const auto& swapChainExtent = swapChainPtr->GetSwapChainExtent();

		const VkFormat depthFormat = SupportUtility::FindDepthFormat();

		InitializeTexture(device, physicalDevice, swapChainExtent.width, swapChainExtent.height,
		                  depthFormat,
		                  VK_IMAGE_TILING_OPTIMAL,
		                  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		depthImageView = CreateImageView(logicalDevice, textureImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

		TransitionImageLayout(depthFormat, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}

	VkImageView DepthTesting::GetImageView() const
	{
		return depthImageView;
	}

	void DepthTesting::Destroy() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto device = logicalDevicePtr->Get();

		VulkanTexture::Destroy(device);

		vkDestroyImageView(device, depthImageView, VK_NULL_HANDLE);
	}

	void DepthTesting::CleanUp() const
	{
		Destroy();
	}
}
