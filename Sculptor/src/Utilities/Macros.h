#pragma once

#define VULKAN_INSTANCE_WRAPPER \
	private: \
	std::weak_ptr<VulkanInstanceWrapper> vulkanInstanceWrapper; \
	public: \
	void SetVulkanInstance(const std::weak_ptr<VulkanInstanceWrapper>& instance) \
	{ \
		this->vulkanInstanceWrapper = instance; \
	}

#define HANDLE_WEAK_VULKAN_INSTANCE_WRAPPER \
	const auto instancePtr = vulkanInstanceWrapper.lock(); \
	if (!instancePtr) \
	{ \
		std::cerr << "Vulkan Instance Wrapper is null.\n"; \
	} \
	const auto& instance = instancePtr->GetInstance();

#define LOGICAL_DEVICE \
	private: \
	std::weak_ptr<LogicalDevice> logicalDevice; \
	public: \
	void SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device) \
	{ \
		this->logicalDevice = device; \
	}

#define HANDLE_WEAK_LOGICAL_DEVICE \
	const auto logicalDevicePtr = logicalDevice.lock(); \
	if (!logicalDevicePtr) \
	{ \
		std::cerr << "Logical Device pointer is null.\n"; \
		return; \
	} \
	const auto& device = logicalDevicePtr->Get();