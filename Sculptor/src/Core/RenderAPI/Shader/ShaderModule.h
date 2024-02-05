#pragma once

namespace Sculptor::Core
{
	class RenderApi;
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class ShaderModule
	{
	public:
		ShaderModule() = default;

		ShaderModule(const std::weak_ptr<LogicalDevice>& device);

		~ShaderModule() = default;

		void DestroyShaderModules() const;

		void CreateShaderStages();

		void SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device);

	private:
		VkShaderModule vertexShaderModule{};

		VkShaderModule fragmentShaderModule{};

		std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};

		std::weak_ptr<LogicalDevice> logicalDevice;

		std::weak_ptr<RenderApi> renderApi;

		friend class GraphicsPipeline;
	};
}