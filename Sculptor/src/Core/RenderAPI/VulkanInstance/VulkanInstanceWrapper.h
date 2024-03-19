#pragma once

namespace Sculptor::Core
{
	class ValidationLayer;
}

namespace Sculptor::Core
{
	class VulkanInstanceWrapper
	{
	public:
		VulkanInstanceWrapper();

		explicit VulkanInstanceWrapper(const VulkanInstanceWrapper&) = delete;

		VulkanInstanceWrapper& operator=(const VulkanInstanceWrapper&) = delete;

		~VulkanInstanceWrapper() = default;

		void CreateInstance(const std::weak_ptr<ValidationLayer>& weakValidationLayer);

		void DestroyInstance() const;

		VkInstance& GetInstance();

	private:
		VkInstance vulkanInstance;
	};
}