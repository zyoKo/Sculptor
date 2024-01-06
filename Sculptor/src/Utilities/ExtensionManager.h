#pragma once

#include "Core/RenderAPI/ValidationLayer/ValidationLayer.h"

namespace Sculptor::Utils
{
	class ExtensionManager
	{
	public:
		static void Initialize(const std::shared_ptr<Core::ValidationLayer>& validationLayerPtr);

		static std::vector<const char*> GetRequiredExtensions(bool printExtensions = false);

		static void PrintAllSupportedExtensions();

	private:
		static inline std::shared_ptr<Core::ValidationLayer> validationLayer;

		static inline bool validationLayerIsInitialized = false;
	};
}
