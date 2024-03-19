#pragma once

#include "Utilities/Macros.h"

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

		ShaderModule(std::weak_ptr<LogicalDevice> device) noexcept;

		~ShaderModule() = default;

		void DestroyShaderModules() const;

		void CreateShaderStages();

		LOGICAL_DEVICE

	private:
		VkShaderModule vertexShaderModule{};

		VkShaderModule fragmentShaderModule{};

		std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};

		FRIEND(GraphicsPipeline)
	};
}