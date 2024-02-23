#pragma once

#include "Buffer.h"

namespace Sculptor::Core
{
	class UniformBuffer
	{
	public:
		UniformBuffer();

		void Create(U64 bufferSize);

		void Update() const;

		const Buffer& GetBuffer() const;

		void Destroy() const;

		explicit operator VkBuffer() const
		{
			return uniformBuffer.GetBuffer();
		}

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
