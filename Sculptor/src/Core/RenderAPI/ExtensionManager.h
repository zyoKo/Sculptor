#pragma once

#include "ValidationLayer/ValidationLayer.h"

namespace Sculptor::Core
{
	class ExtensionManager
	{
	public:
		static void Initialize(const std::shared_ptr<ValidationLayer>& validationLayerPtr);

		static std::vector<const char*> GetRequiredExtensions();

		static void PrintAllSupportedExtensions();

	private:
		static inline std::shared_ptr<ValidationLayer> validationLayer;

		static inline bool validationLayerIsInitialized = false;
	};
}
