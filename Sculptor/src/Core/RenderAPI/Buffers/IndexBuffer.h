#pragma once

#include "Buffer.h"

namespace Sculptor::Core
{
	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer() = default;

		void Create(const BufferProperties& bufferProperties) override;

		void BindBuffer(VkCommandBuffer commandBuffer) const override;
	};
}
