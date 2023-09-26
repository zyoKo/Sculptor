#pragma once

namespace Sculptor::Core
{
	class WindowsWindow;

	class VulkanInstanceWrapper;
}

namespace Sculptor::Windows
{
	class VulkanWindowSurface
	{
	public:
		VulkanWindowSurface();

		~VulkanWindowSurface() = default;

		void CreateWindowSurface(const std::shared_ptr<Core::VulkanInstanceWrapper>& vulkanInstanceWrapper, 
			const std::shared_ptr<Core::WindowsWindow>& nativeWindow);

		void CleanUp(const std::shared_ptr<Core::VulkanInstanceWrapper>& vulkanInstanceWrapper) const;

		const VkSurfaceKHR& GetSurface() const;

		VkSurfaceKHR& GetSurface();

	private:
		VkSurfaceKHR windowSurface;
	};
}
