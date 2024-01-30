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

		RenderApi(const std::weak_ptr<SwapChain>& swapChain, const std::weak_ptr<LogicalDevice>& logicalDevice);

		~RenderApi() = default;

		void Create();

		void SetSwapChain(const std::weak_ptr<SwapChain>& swapChain);

		void SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device);

		void CleanUp() const;

		static bool IsDeviceSuitable(const LogicalDevice& device, const std::weak_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface);

		void DrawFrame();

	private:
		VkRenderPass renderPass;

		std::weak_ptr<SwapChain> swapChain;

		std::weak_ptr<LogicalDevice> logicalDevice;

		friend class GraphicsPipeline;

		friend class FrameBuffer;

		friend class CommandBuffer;
	};
}
