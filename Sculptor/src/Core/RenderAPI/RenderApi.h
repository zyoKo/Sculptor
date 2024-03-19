#pragma once

#include "Devices/LogicalDevice.h"
#include "Devices/QueueFamily/QueueFamilies.h"
#include "SwapChains/SwapChain.h"

namespace Sculptor::Core
{
	class RenderApi
	{
	public:
		RenderApi();

		RenderApi(std::weak_ptr<LogicalDevice> logicalDevice, std::weak_ptr<SwapChain> swapChain) noexcept;

		~RenderApi() = default;

		void Create();

		void CleanUp() const;

		VkRenderPass GetRenderPass() const;

		static bool IsDeviceSuitable(const LogicalDevice& device, const std::weak_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface);

		SWAP_CHAIN

		LOGICAL_DEVICE

	private:
		VkRenderPass renderPass;

		FRIEND(GraphicsPipeline)

		FRIEND(FrameBuffer)

		FRIEND(CommandBuffer)
	};
}
