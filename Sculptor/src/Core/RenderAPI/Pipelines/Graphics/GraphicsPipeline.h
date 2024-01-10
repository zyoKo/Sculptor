#pragma once

namespace Sculptor::Core
{
	class ShaderModule;

	class SwapChain;

	class RenderApi;

	class LogicalDevice;
}

namespace Sculptor::Core
{
	class GraphicsPipeline
	{
	public:
		GraphicsPipeline() = default;

		GraphicsPipeline(const std::weak_ptr<RenderApi>& renderApi, const std::weak_ptr<SwapChain>& swapChain,
			const std::weak_ptr<LogicalDevice>& device);

		void CreateGraphicsPipeline();

		void CleanUp() const;

		void SetRenderApi(const std::weak_ptr<RenderApi>& renderApi);

		void SetSwapChain(const std::weak_ptr<SwapChain>& swapChain);

		void SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device);

	private:
		VkPipelineLayout pipelineLayout{};

		VkPipeline graphicsPipeline{};

		std::shared_ptr<ShaderModule> shaderModule;

		std::weak_ptr<RenderApi> renderApi;

		std::weak_ptr<SwapChain> swapChain;

		std::weak_ptr<LogicalDevice> logicalDevice;
	};
}
