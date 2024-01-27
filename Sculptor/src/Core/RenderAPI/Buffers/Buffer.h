#pragma once

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
		Buffer() = default;

		~Buffer() = default;

		void Create(const BufferProperties& properties);

		static void Copy(const Buffer& source, const Buffer& destination, VkDeviceSize size);

		LOGICAL_DEVICE

		const VkBuffer& Get() const;

		const VkDeviceMemory& GetBufferMemory() const;

		void Destroy() const;

	private:
		VkBuffer buffer{};

		VkDeviceMemory bufferMemory{};
	};
}
