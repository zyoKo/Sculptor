#pragma once

namespace Sculptor::Core
{
	class WindowsWindow;

	class ValidationLayer;

	class VulkanInstanceWrapper;
}

namespace Sculptor::Windows
{
	class VulkanWindowSurface
	{
	public:
		VulkanWindowSurface();

		~VulkanWindowSurface() = default;

		void CreateWindowSurface();

		void CleanUp() const;

		const VkSurfaceKHR& GetSurface() const;

		VkSurfaceKHR& GetSurface();

		void SetNativeWindow(const std::shared_ptr<Core::WindowsWindow>& nativeWindow);

		const std::shared_ptr<Core::WindowsWindow>& GetNativeWindow() const;

		void SetVulkanInstanceWrapper(const std::shared_ptr<Core::VulkanInstanceWrapper>& vulkanInstanceWrap);

		const std::shared_ptr<Core::VulkanInstanceWrapper>& GetVulkanInstanceWrapper() const;

		void SetValidationLayer(const std::shared_ptr<Core::ValidationLayer>& validationLayer);

		const std::shared_ptr<Core::ValidationLayer>& GetValidationLayer() const;

	private:
		VkSurfaceKHR windowSurface;

		std::shared_ptr<Core::WindowsWindow> nativeWindow;

		std::shared_ptr<Core::VulkanInstanceWrapper> vulkanInstanceWrapper;

		std::shared_ptr<Core::ValidationLayer> validationLayer;
	};
}
