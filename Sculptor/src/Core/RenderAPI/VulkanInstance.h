#pragma once

#include "ValidationLayer/ValidationLayer.h"

namespace Sculptor::Core
{
	class VulkanInstance
	{
	public:
		VulkanInstance();

		~VulkanInstance() = default;
	private:
		VkInstance vulkanInstance;

		void CreateInstance(const std::unique_ptr<ValidationLayer>& validationLayer);

		void DestroyInstance() const;

		static std::vector<const char*> GetRequiredExtensions(const std::unique_ptr<ValidationLayer>& validationLayer);

		static void PrintAllSupportedExtensions();

		static void PrintRequiredGLFWExtensions(const std::vector<const char*>& requiredExtensions);
	};
}