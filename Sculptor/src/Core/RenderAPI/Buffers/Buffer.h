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
	class Buffer : public IBaseBuffer
	{
	public:
		Buffer();

		~Buffer() override = default;

		void Create(const BufferProperties& properties) override;

		void Destroy() const override;

		void BindBuffer(VkCommandBuffer commandBuffer) const override;

		void BindBufferMemory(VkDeviceSize bufferSize = 0) const override;
		
		VkBuffer GetBuffer() const override;

		VkDeviceMemory GetBufferMemory() const override;

		LOGICAL_DEVICE

	private:
		VkBuffer buffer;

		VkDeviceMemory bufferMemory;

		friend class IndexBuffer;

		friend class VertexBuffer;

		friend class UniformBuffer;
	};
}
