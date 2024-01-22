#include <SculptorPch.h>

#include "RenderApi.h"

#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	RenderApi::RenderApi()
		:	renderPass(nullptr)
	{ }

	RenderApi::RenderApi(const std::weak_ptr<SwapChain>& swapChain, const std::weak_ptr<LogicalDevice>& logicalDevice)
		:	renderPass(nullptr),
			swapChain(swapChain),
			logicalDevice(logicalDevice)
	{ }

	void RenderApi::CreateRenderPass()
	{
		const auto swapChainPtr = swapChain.lock();
		if (!swapChainPtr)
		{
			std::cerr << "Failed to create Render Pass!" << std::endl;
			return;
		}

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = swapChainPtr->swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		// since we are not using stencil buffers
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentReference;
		colorAttachmentReference.attachment = 0;	// index of VkAttachmentDescription, since only 1 (above) then index is 0
		colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subPass{};
		subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subPass.colorAttachmentCount = 1;
		subPass.pColorAttachments = &colorAttachmentReference;
		subPass.pInputAttachments = nullptr; // attachments that are read from a shader
		subPass.pResolveAttachments = nullptr; // attachments used for multi-sampling color attachments
		subPass.pDepthStencilAttachment = nullptr; // attachments for depth and stencil data
		subPass.pPreserveAttachments = nullptr; // attachments that are not used by this subpass, but for which data must be preserved

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subPass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "Logical Device is nullptr!" << std::endl;
			std::cerr << "Cannot create Render Pass!" << std::endl;
			return;
		}
		const auto& device = logicalDevicePtr->Get();

		const auto result = vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass);
		S_ASSERT(result != VK_SUCCESS, "Failed to create Graphics Pipeline.");
	}

	void RenderApi::SetSwapChain(const std::weak_ptr<SwapChain>& swapChain)
	{
		this->swapChain = swapChain;
	}

	void RenderApi::SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device)
	{
		this->logicalDevice = device;
	}

	void RenderApi::CleanUp() const
	{
		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "Logical Device is nullptr!" << std::endl;
			std::cerr << "Cannot cleanup pipeline Layout and Render Pass!" << std::endl;
			return;
		}
		const auto& device = logicalDevicePtr->Get();

		vkDestroyRenderPass(device, renderPass, nullptr);
	}

	bool RenderApi::IsDeviceSuitable(const LogicalDevice& device,
	                                 const std::weak_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface)
	{
		const auto& queueFamilies = device.GetQueueFamilies();
		const bool isDeviceSuitable = queueFamilies.GetQueueFamilyIndices().IsComplete();

		const auto physicalDevice = device.GetPhysicalDevice().lock();
		if (!physicalDevice)
		{
			std::cerr << "Initialize Physical Device before checking device suitability." << std::endl;
			return false;
		}

		const bool checkDeviceExtensionSupport = device.CheckDeviceExtensionSupport();

		bool swapChainAdequate = false;
		if (checkDeviceExtensionSupport)
		{
			const auto swapChainSupportDetails = SwapChain::QuerySwapChainSupport(vulkanWindowSurface, device.GetPhysicalDevice());
			swapChainAdequate = !swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty();
		}

		return isDeviceSuitable && checkDeviceExtensionSupport && swapChainAdequate;
	}

	void RenderApi::DrawFrame()
	{

	}
}
