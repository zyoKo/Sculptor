#pragma once

#include "Core/RenderAPI/Interfaces/BaseBuffer.h"
#include "Structures/BufferProperties.h"
#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class Buffer : public BaseBuffer
	{
	public:
		Buffer() = default;

		~Buffer() override = default;

		void Create(const BufferProperties& properties) override;

		void Destroy() const override;

		void Copy(VkDeviceSize size) override { /* TODO: Fix this later */ }

		void BindBufferMemory() const override;

		LOGICAL_DEVICE
	};
}
