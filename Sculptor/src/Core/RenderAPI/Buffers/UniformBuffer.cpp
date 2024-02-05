#include <SculptorPch.h>

#include <glm/gtc/matrix_transform.hpp>

#include "UniformBuffer.h"


#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/Locators/SwapChainLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Structures/UniformBufferObject.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	UniformBuffer::UniformBuffer()
		:	uniformBufferMapped(nullptr)
	{ }

	void UniformBuffer::Create(const BufferProperties& properties)
	{
		LOGICAL_DEVICE_LOCATOR

		Buffer::Create(properties);

		vkMapMemory(device, bufferMemory, 0, properties.bufferSize, 0, &uniformBufferMapped);
	}

	void UniformBuffer::Update() const
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		const auto currentTime = std::chrono::high_resolution_clock::now();
		const float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo{};

		ubo.model = glm::rotate(
			glm::mat4(1.0f), 
			time * glm::radians(90.0f), 
			glm::vec3(0.0f, 0.0, 1.0f));

		ubo.view = glm::lookAt(
			glm::vec3(2.0f, 2.0f, 2.0f), 
			glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3(0.0f, 0.0f, 1.0f));

		const auto swapChainPtr = SwapChainLocator::GetSwapChain().lock();
		S_ASSERT(!swapChainPtr, "Swap Chain reference is null.\n");
		const auto& swapChainExtent = swapChainPtr->GetSwapChainExtent();

		ubo.projection = glm::perspective(
			glm::radians(45.0f), 
			static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height), 
			0.1f, 
			10.0f);

		ubo.projection[1][1] *= -1;

		memcpy(uniformBufferMapped, &ubo, sizeof(ubo));
	}
}
