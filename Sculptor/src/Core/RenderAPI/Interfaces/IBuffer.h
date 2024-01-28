#pragma once

#include "Core/RenderAPI/Buffers/Structures/BufferProperties.h"

namespace Sculptor::Core
{
	class IBuffer
	{
	public:
		virtual ~IBuffer() = default;

		virtual void Create(const BufferProperties& bufferProperties) = 0;

		virtual void Copy(VkDeviceSize size) = 0;

		virtual void BindBuffer(const VkCommandBuffer& commandBuffer) const { }

		virtual void BindBufferMemory() const = 0;

		virtual void Destroy() const = 0;

		const VkBuffer& GetBuffer() const
		{
			return buffer;
		}

		VkBuffer& GetBuffer()
		{
			return buffer;
		}

		const VkDeviceMemory& GetBufferMemory() const
		{
			return bufferMemory;
		}

		VkDeviceMemory& GetBufferMemory()
		{
			return bufferMemory;
		}

	protected:
		IBuffer() = default;

		VkBuffer buffer{};

		VkDeviceMemory bufferMemory{};
	};
}
