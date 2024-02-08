#pragma once

// TODO: Use GetShared Class to deal with weak pointer class

// Friend
#define FRIEND(className) friend class className;

/////////////////////////////////////////////////////////////////////////////
// Vulkan Instance Wrapper //////////////////////////////////////////////////
#define VULKAN_INSTANCE_WRAPPER \
	private: \
		std::weak_ptr<VulkanInstanceWrapper> vulkanInstanceWrapper; \
	public: \
		void SetVulkanInstance(std::weak_ptr<VulkanInstanceWrapper> instance) noexcept \
		{ \
			this->vulkanInstanceWrapper = std::move(instance); \
		}

#define HANDLE_WEAK_VULKAN_INSTANCE_WRAPPER \
	const auto instancePtr = vulkanInstanceWrapper.lock(); \
	if (!instancePtr) \
	{ \
		std::cerr << "Vulkan Instance Wrapper is null.\n"; \
		__debugbreak(); \
	} \
	const auto& instance = instancePtr->GetInstance();
// Vulkan Instance Wrapper //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Logical Device ///////////////////////////////////////////////////////////
#define LOGICAL_DEVICE \
	private: \
		std::weak_ptr<LogicalDevice> logicalDevice; \
	public: \
		void SetLogicalDevice(std::weak_ptr<LogicalDevice> device) noexcept \
		{ \
			this->logicalDevice = std::move(device); \
		}

#define LOGICAL_DEVICE_LOCATOR \
	const auto logicalDevice = LogicalDeviceLocator::GetLogicalDevice(); \
	const auto logicalDevicePtr = logicalDevice.lock(); \
	if (!logicalDevicePtr) \
	{ \
		std::cerr << "Logical Device pointer is null.\n"; \
		__debugbreak(); \
	} \
	const auto& device = logicalDevicePtr->Get();

#define HANDLE_WEAK_LOGICAL_DEVICE_DEPRECATED \
	const auto logicalDevicePtr = logicalDevice.lock(); \
	if (!logicalDevicePtr) \
	{ \
		std::cerr << "Logical Device pointer is null.\n"; \
		__debugbreak(); \
	} \
	const auto& device = logicalDevicePtr->Get();
// Logical Device ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Command Buffer ///////////////////////////////////////////////////////////
#define COMMAND_BUFFER \
	private: \
		std::weak_ptr<CommandBuffer> commandBuffer; \
	public: \
		void SetCommandBuffer(std::weak_ptr<CommandBuffer> buffer) noexcept \
		{ \
			this->commandBuffer = std::move(buffer); \
		}

#define HANDLE_WEAK_COMMAND_BUFFER \
	const auto weakCommandBufferPtr = commandBuffer.lock(); \
	if (!weakCommandBufferPtr) \
	{ \
		std::cerr << "Command Buffer pointer is null.\n"; \
		__debugbreak(); \
	} \
	const auto& cmdBuffer = weakCommandBufferPtr->GetBuffer();
// Command Buffer ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Command Pool /////////////////////////////////////////////////////////////
#define COMMAND_POOL_LOCATOR \
	const auto commandPool = CommandPoolLocator::GetCommandPool(); \
	const auto commandPoolPtr = commandPool.lock(); \
	if (!commandPoolPtr) \
	{ \
		std::cerr << "Command Pool pointer is null.\n"; \
		__debugbreak(); \
	} \
	const auto& cmdPool = commandPoolPtr->Get();
// Command Pool /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Swap Chain ///////////////////////////////////////////////////////////////
#define SWAP_CHAIN \
	private: \
		std::weak_ptr<SwapChain> swapChain; \
	public: \
		void SetSwapChain(std::weak_ptr<SwapChain> swapChainPtr) noexcept \
		{ \
			this->swapChain = std::move(swapChainPtr); \
		}
// Swap Chain ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////