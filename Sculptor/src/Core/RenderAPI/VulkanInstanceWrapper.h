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

		void CreateInstance(const std::shared_ptr<ValidationLayer>& validationLayer);

		void DestroyInstance() const;

		VkInstance& GetInstance();

		const VkInstance& GetInstance() const;

	private:
		VkInstance vulkanInstance;
	};
}