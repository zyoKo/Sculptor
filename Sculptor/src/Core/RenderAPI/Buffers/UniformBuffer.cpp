#include <SculptorPch.h>

#include <glm/gtc/matrix_transform.hpp>

#include "UniformBuffer.h"

#include "Core/Locators/SwapChainLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Structures/UniformBufferObject.h"

namespace Sculptor::Core
{
	UniformBuffer::UniformBuffer(std::weak_ptr<LogicalDevice> device) noexcept
		:	logicalDevice(std::move(device)),
			uniformBufferMapped(nullptr)
	{ }

	VkBuffer UniformBuffer::Create(U64 bufferSize)
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto device = logicalDevicePtr->Get();

		uniformBufferProperties.bufferSize = bufferSize;
		uniformBuffer.Create(uniformBufferProperties);

		VK_CHECK(vkMapMemory(device, uniformBuffer.GetBufferMemory(), 0, bufferSize, 0, &uniformBufferMapped),
			"Failed to map memory in Uniform Buffer pointer.")

		return uniformBuffer.GetBuffer();
	}

	void UniformBuffer::Update() const
	{
		GetShared<SwapChain> swapChainPtr{ SwapChainLocator::GetSwapChain() };
		const auto& swapChainExtent = swapChainPtr->GetSwapChainExtent();

		static auto startTime = std::chrono::high_resolution_clock::now();

		const auto currentTime = std::chrono::high_resolution_clock::now();
		const float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo{};

		//ubo.model = glm::mat4(1.0f);

		ubo.model = glm::rotate(
			glm::mat4(1.0f), 
			time * glm::radians(90.0f), 
			glm::vec3(0.0f, 0.0, 1.0f));

		ubo.view = glm::lookAt(
			glm::vec3(2.0f, 2.0f, 2.0f), 
			glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3(0.0f, 0.0f, 1.0f));

		ubo.projection = glm::perspective(
			glm::radians(45.0f), 
			static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height), 
			0.1f, 
			10.0f);

		ubo.projection[1][1] *= -1;

		memcpy(uniformBufferMapped, &ubo, sizeof(ubo));
	}

	const Buffer& UniformBuffer::GetBuffer() const
	{
		return uniformBuffer;
	}

	void UniformBuffer::Destroy() const
	{
		uniformBuffer.Destroy();
	}

	UniformBuffer::operator VkBuffer() const
	{
		return uniformBuffer.GetBuffer();
	}
}
