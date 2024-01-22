#include <SculptorPch.h>

#include "ShaderModule.h"

#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Utilities/Utilites.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	ShaderModule::ShaderModule(const std::weak_ptr<LogicalDevice>& device)
		:	logicalDevice(device)
	{ }

	VkShaderModule ShaderModule::CreateShaderModule(const std::vector<char>& code) const
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		const auto& logicalDevicePtr = logicalDevice.lock();
		S_ASSERT(!logicalDevicePtr, "Failed to create shader module.");
		const auto& device = logicalDevicePtr->Get();

		VkShaderModule shaderModule{};

		const auto result = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
		S_ASSERT(result != VK_SUCCESS, "Failed to create shader module.");

		return shaderModule;
	}

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
		const auto vertexShaderCode = Utilities::ReadFile("./src/Assets/Shaders/vert.spv");
		const auto fragmentShaderCode = Utilities::ReadFile("./src/Assets/Shaders/frag.spv");

		vertexShaderModule = CreateShaderModule(vertexShaderCode);
		fragmentShaderModule = CreateShaderModule(fragmentShaderCode);

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
