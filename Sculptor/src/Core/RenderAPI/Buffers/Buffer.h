#pragma once

#include "Core/RenderAPI/Interfaces/IBaseBuffer.h"
#include "Structures/BufferProperties.h"
#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class Buffer
	{
	public:
		Buffer();

		~Buffer() = default;

		void Create(const BufferProperties& properties);

		void Destroy() const;

		void BindBufferMemory(VkDeviceSize bufferSize = 0) const;
		
		VkBuffer& GetBuffer();

		const VkBuffer& GetBuffer() const;

		VkDeviceMemory& GetBufferMemory();

		const VkDeviceMemory& GetBufferMemory() const;

		explicit operator VkBuffer() const
		{
			return buffer;
		}

		LOGICAL_DEVICE

	private:
		VkBuffer buffer;

		VkDeviceMemory bufferMemory;

		friend class IndexBuffer;

		friend class VertexBuffer;

		friend class UniformBuffer;
	};
}
