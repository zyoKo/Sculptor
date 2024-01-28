#pragma once

#include "Core/RenderAPI/Interfaces/IBuffer.h"
#include "Structures/BufferProperties.h"
#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class Buffer : public IBuffer
	{
	public:
		Buffer() = default;

		~Buffer() override = default;

		void Create(const BufferProperties& properties) override;

		static void Copy(const Buffer& source, const Buffer& destination, VkDeviceSize size);

		void Destroy() const override;

		LOGICAL_DEVICE

		void Copy(VkDeviceSize size) override { /* TODO: Fix this later */ }

		void BindBufferMemory() const override;
	};
}
