#pragma once

#include "Core/RenderAPI/Buffers/Structures/BufferProperties.h"

namespace Sculptor::Core
{
	class IBaseBuffer
	{
	public:
		virtual ~IBaseBuffer() = default;

		virtual void Create(const BufferProperties& bufferProperties) = 0;

		virtual void BindBuffer(VkCommandBuffer commandBuffer) const = 0;

		virtual void BindBufferMemory(VkDeviceSize bufferSize = 0) const = 0;

		virtual void Destroy() const = 0;

		virtual VkBuffer GetBuffer() const = 0;

		virtual VkDeviceMemory GetBufferMemory() const = 0;
	};
}
