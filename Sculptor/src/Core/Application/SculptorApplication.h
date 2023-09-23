#pragma once

#include <optional>

#include "Core/RenderAPI/Devices/PhysicalDevice.h"
#include "Core/Windows/Window.h"
#include "Core/RenderAPI/ValidationLayer/ValidationLayer.h"

namespace Sculptor::Core
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;

		bool IsGraphicsFamilyComplete() const { return graphicsFamily.has_value(); }
	};

	class SculptorApplication
	{
	public:
		SculptorApplication();

		SculptorApplication(const SculptorApplication&) = delete;

		SculptorApplication& operator=(const SculptorApplication&) = delete;

		~SculptorApplication() = default;

		void Sculpt();

	private:
		std::unique_ptr<Window> window;

		WindowProperties windowProperties;

		std::shared_ptr<VulkanInstanceWrapper> vulkanInstanceWrapper;

		std::shared_ptr<ValidationLayer> validationLayer;

		std::shared_ptr<PhysicalDevice> physicalDevice;

		void InitializeWindow() const;

		void InitializeVulkan();

		void MainLoop() const;

		void CleanUp() const;

		// Queue Families
		VkQueue graphicsQueue;

		static QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device);

		static bool IsDeviceSuitable(const VkPhysicalDevice& device);

		// Logical Device
		VkDevice device;

		void CreateLogicalDevice();
	};
}
