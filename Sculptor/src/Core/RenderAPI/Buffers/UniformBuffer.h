#pragma once
#include "Buffer.h"

namespace Sculptor::Core
{
	class UniformBuffer : public Buffer
	{
	public:
		UniformBuffer();

		void Create(const BufferProperties& properties) override;

		void Update() const;

	private:
		void* uniformBufferMapped;
	};
}
