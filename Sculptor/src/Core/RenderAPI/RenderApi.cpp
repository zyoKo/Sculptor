#include <SculptorPch.h>

#include <utility>

#include "RenderApi.h"
#include "Utility/CreateInfo.h"
#include "Utility/SupportUtility.h"

namespace Sculptor::Core
{
	RenderApi::RenderApi()
		:	renderPass(VK_NULL_HANDLE)
	{ }

	RenderApi::RenderApi(std::weak_ptr<LogicalDevice> logicalDevice, std::weak_ptr<SwapChain> swapChain) noexcept
		:	swapChain(std::move(swapChain)),
			logicalDevice(std::move(logicalDevice)),
			renderPass(VK_NULL_HANDLE)
	{ }

	void RenderApi::Create()
	{
		GetShared<SwapChain> swapChainPtr { swapChain };

		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		VkAttachmentDescription depthAttachment{
			.format			= SupportUtility::FindDepthFormat(),
			.samples		= VK_SAMPLE_COUNT_1_BIT,
			.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp		= VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout	= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
		};

		VkAttachmentReference depthAttachmentReference{
			.attachment = 1,
			.layout		= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
		};

		const VkAttachmentDescription colorAttachment{
			.format			= swapChainPtr->swapChainImageFormat,
			.samples		= VK_SAMPLE_COUNT_1_BIT,
			.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp		= VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE, // since we are not using stencil buffers
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout	= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		};

		VkAttachmentReference colorAttachmentReference{
			.attachment = 0,	// index of VkAttachmentDescription, since only 1 (above) then index is 0
			.layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		};

		const VkSubpassDescription subPassDescription{
			.pipelineBindPoint		 = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.inputAttachmentCount	 = 0,
			.pInputAttachments		 = VK_NULL_HANDLE, // attachments that are read from a shader
			.colorAttachmentCount	 = 1,
			.pColorAttachments		 = &colorAttachmentReference,
			.pResolveAttachments	 = VK_NULL_HANDLE, // attachments used for multi-sampling color attachments
			.pDepthStencilAttachment = &depthAttachmentReference, // attachments for depth and stencil data
			.pPreserveAttachments	 = VK_NULL_HANDLE, // attachments that are not used by this sub-pass, but for which data must be preserved
		};

		constexpr VkSubpassDependency dependency{
			.srcSubpass		= VK_SUBPASS_EXTERNAL,
			.dstSubpass		= 0,
			.srcStageMask	= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
			.dstStageMask	= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
			.srcAccessMask	= 0,
			.dstAccessMask	= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
		};

		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

		const auto renderPassInfo = CreateInfo<VkRenderPassCreateInfo>({
			.attachmentCount	= static_cast<U32>(attachments.size()),
			.pAttachments		= attachments.data(),
			.subpassCount		= 1,
			.pSubpasses			= &subPassDescription,
			.dependencyCount	= 1,
			.pDependencies		= &dependency
		});

		VK_CHECK(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass), "Failed to create Graphics Pipeline.")
	}

	void RenderApi::CleanUp() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		vkDestroyRenderPass(device, renderPass, nullptr);
	}

	VkRenderPass RenderApi::GetRenderPass() const
	{
		return renderPass;
	}

	bool RenderApi::IsDeviceSuitable(const LogicalDevice& device,
	                                 const std::weak_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface)
	{
		GetShared<PhysicalDevice> physicalDevice{ device.GetPhysicalDevice() };
		const bool isAnisotropyEnabled = physicalDevice->GetDeviceFeatures().samplerAnisotropy;

		const auto& queueFamilies = device.GetQueueFamilies();
		const bool isDeviceSuitable = queueFamilies.GetQueueFamilyIndices().IsComplete();

		const bool checkDeviceExtensionSupport = device.CheckDeviceExtensionSupport();

		bool swapChainAdequate = false;
		if (checkDeviceExtensionSupport)
		{
			const auto swapChainSupportDetails = SwapChain::QuerySwapChainSupport(vulkanWindowSurface, device.GetPhysicalDevice());
			swapChainAdequate = !swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty();
		}

		return isDeviceSuitable && checkDeviceExtensionSupport && swapChainAdequate && isAnisotropyEnabled;
	}
}
