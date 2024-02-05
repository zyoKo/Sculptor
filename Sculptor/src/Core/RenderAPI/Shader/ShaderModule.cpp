#include <SculptorPch.h>

#include "ShaderModule.h"

#include "Core/Core.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Utilities/Macros.h"
#include "Utilities/Utilities.h"
#include "Utilities/Logger/Assert.h"
#include "Core/Locators/LogicalDeviceLocator.h"

namespace Sculptor::Core
{
	ShaderModule::ShaderModule(const std::weak_ptr<LogicalDevice>& device)
		:	logicalDevice(device)
	{ }

	void ShaderModule::DestroyShaderModules() const
	{
		const auto& logicalDevicePtr = logicalDevice.lock();
		S_ASSERT(!logicalDevicePtr, "Failed to create shader module.");

		const auto& device = logicalDevicePtr->Get();

		vkDestroyShaderModule(device, fragmentShaderModule, nullptr);
		vkDestroyShaderModule(device, vertexShaderModule, nullptr);
	}

	void ShaderModule::CreateShaderStages()
	{
		LOGICAL_DEVICE_LOCATOR

		const auto vertexShaderCode = Utilities::ReadFile("./Assets/Shaders/vert.spv");
		const auto fragmentShaderCode = Utilities::ReadFile("./Assets/Shaders/frag.spv");

		vertexShaderModule	 = Utilities::CreateShaderModule(vertexShaderCode, device);
		fragmentShaderModule = Utilities::CreateShaderModule(fragmentShaderCode, device);

		VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
		vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertexShaderStageInfo.module = vertexShaderModule;
		vertexShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
		fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragmentShaderStageInfo.module = fragmentShaderModule;
		fragmentShaderStageInfo.pName = "main";

		shaderStages = { vertexShaderStageInfo, fragmentShaderStageInfo };
	}

	void ShaderModule::SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device)
	{
		this->logicalDevice = device;
	}
}
