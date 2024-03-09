#include <SculptorPch.h>

#include "GLFW/glfw3.h"

#include "VulkanInstanceWrapper.h"

#include "Core/RenderAPI/Data/Constants.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"
#include "Utilities/ExtensionManager.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	VulkanInstanceWrapper::VulkanInstanceWrapper()
		:	vulkanInstance(VK_NULL_HANDLE)
	{ }

	void VulkanInstanceWrapper::CreateInstance(const std::weak_ptr<ValidationLayer>& weakValidationLayer)
	{
		GetShared<ValidationLayer> validationLayerPtr{ weakValidationLayer };
		S_ASSERT(!validationLayerPtr->RequestValidationLayer(), "Failed to request validation Layer!")

		// Create Application Info
		const auto applicationInfo = CreateInfo<VkApplicationInfo>({
			.pApplicationName = APPLICATION::NAME,
			.applicationVersion = VK_MAKE_VERSION(APPLICATION::MAJOR, APPLICATION::MINOR, APPLICATION::PATCH),
			.pEngineName = ENGINE::NAME,
			.engineVersion = VK_MAKE_VERSION(ENGINE::MAJOR, ENGINE::MINOR, ENGINE::PATCH),
			.apiVersion = VK_MAKE_VERSION(API::MAJOR, API::MINOR, API::PATCH)
		});

		// Create Info needs both applicationInfo and glfwExtensions
		const auto requiredExtensions = Utils::ExtensionManager::GetRequiredExtensions();

		// Get GLFW Extension Count and Required Extensions
		auto createInfo = CreateInfo<VkInstanceCreateInfo>({
			.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
			.pApplicationInfo = &applicationInfo,
			.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
			.ppEnabledExtensionNames = requiredExtensions.data(),
		});

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (validationLayerPtr != nullptr && validationLayerPtr->IsEnabled())
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayerPtr->GetValidationLayersArray().size());
			createInfo.ppEnabledLayerNames = validationLayerPtr->GetValidationLayersArray().data();

			validationLayerPtr->PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		// Finally create VulkanInstanceWrapper
		VK_CHECK(vkCreateInstance(&createInfo, nullptr, &vulkanInstance), "Failed to Create Vulkan Instance!")
	}

	void VulkanInstanceWrapper::DestroyInstance() const
	{
		vkDestroyInstance(vulkanInstance, nullptr);
	}

	VkInstance& VulkanInstanceWrapper::GetInstance()
	{
		return vulkanInstance;
	}

	const VkInstance& VulkanInstanceWrapper::GetInstance() const
	{
		return vulkanInstance;
	}
}
