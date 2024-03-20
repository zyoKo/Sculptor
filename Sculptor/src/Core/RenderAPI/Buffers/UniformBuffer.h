#pragma once

#include "Buffer.h"
#include "Components/Camera/Camera.h"

namespace Sculptor::Core
{
	class LogicalDevice;
};

namespace Sculptor::Core
{
	class UniformBuffer
	{
	public:
		UniformBuffer(std::weak_ptr<LogicalDevice> device) noexcept;

		[[nodiscard]] VkBuffer Create(U64 bufferSize);

		void Update() const;

		const Buffer& GetBuffer() const;

		void Destroy() const;

		explicit operator VkBuffer() const;

		LOGICAL_DEVICE

	private:
		Buffer uniformBuffer;

		void* uniformBufferMapped;

		inline static BufferProperties uniformBufferProperties{
			0,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};
	};
}
