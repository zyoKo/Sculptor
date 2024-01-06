#pragma once

namespace Sculptor::Windows
{
	class VulkanWindowSurface;
}

namespace Sculptor::Core
{
	class WindowsWindow;

	class VulkanInstanceWrapper;

	class ValidationLayer;

	class LogicalDevice;

	class SwapChains;
}

namespace Sculptor::Core
{
	class SculptorApplication
	{
	public:
		SculptorApplication();

		explicit SculptorApplication(const SculptorApplication&) = delete;

		SculptorApplication& operator=(const SculptorApplication&) = delete;

		~SculptorApplication() = default;

		void Sculpt() const;

	private:
		std::shared_ptr<WindowsWindow> window;

		std::shared_ptr<VulkanInstanceWrapper> vulkanInstanceWrapper;

		std::shared_ptr<ValidationLayer> validationLayer;

		std::shared_ptr<Windows::VulkanWindowSurface> windowSurface;

		std::shared_ptr<LogicalDevice> logicalDevice;

		std::shared_ptr<SwapChains> swapChains;

		void InitializeWindow() const;

		void InitializeVulkan() const;

		void MainLoop() const;

		void CleanUp() const;
	};
}
