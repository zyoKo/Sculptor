#pragma once

#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Devices/PhysicalDevice.h"
#include "Core/Windows/Window.h"
#include "Core/RenderAPI/ValidationLayer/ValidationLayer.h"

namespace Sculptor::Core
{
	class SculptorApplication
	{
	public:
		SculptorApplication();

		SculptorApplication(const SculptorApplication&) = delete;

		SculptorApplication& operator=(const SculptorApplication&) = delete;

		~SculptorApplication() = default;

		void Sculpt() const;

	private:
		std::unique_ptr<Window> window;

		WindowProperties windowProperties;

		std::shared_ptr<VulkanInstanceWrapper> vulkanInstanceWrapper;

		std::shared_ptr<ValidationLayer> validationLayer;

		std::shared_ptr<LogicalDevice> logicalDevice;

		void InitializeWindow() const;

		void InitializeVulkan() const;

		void MainLoop() const;

		void CleanUp() const;
	};
}
