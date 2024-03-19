#include <SculptorPch.h>

#include "ShaderModule.h"

#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Utilities/Utilities.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"

namespace Sculptor::Core
{
	ShaderModule::ShaderModule(std::weak_ptr<LogicalDevice> device) noexcept
		:	logicalDevice(std::move(device))
	{ }

	void ShaderModule::DestroyShaderModules() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		vkDestroyShaderModule(device, fragmentShaderModule, nullptr);
		vkDestroyShaderModule(device, vertexShaderModule, nullptr);
	}

	void ShaderModule::CreateShaderStages()
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		const auto vertexShaderCode   = Utilities::ReadFile("./Assets/Shaders/vert.spv");
		const auto fragmentShaderCode = Utilities::ReadFile("./Assets/Shaders/frag.spv");

		vertexShaderModule	 = Utilities::CreateShaderModule(vertexShaderCode, device);
		fragmentShaderModule = Utilities::CreateShaderModule(fragmentShaderCode, device);

		const auto vertexShaderStageInfo = CreateInfo<VkPipelineShaderStageCreateInfo>({
			.stage = VK_SHADER_STAGE_VERTEX_BIT,
			.module = vertexShaderModule,
			.pName = "main"
		});

		const auto fragmentShaderStageInfo = CreateInfo<VkPipelineShaderStageCreateInfo>({
			.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			.module = fragmentShaderModule,
			.pName = "main"
		});

		shaderStages = { vertexShaderStageInfo, fragmentShaderStageInfo };
	}
}
