#pragma once

#include "Buffer.h"

namespace Sculptor::Core
{
	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer() = default;

		void Create(const BufferProperties& bufferProperties);

		void BindBuffer(const VkCommandBuffer& commandBuffer) const override;

		void CleanUp() const;
	};
}
