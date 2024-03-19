#pragma once

#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
	class WindowsWindow;
}

namespace Sculptor::Core
{
	class EngineTools final
	{
	public:
		EngineTools(std::weak_ptr<LogicalDevice> device) noexcept;

		explicit EngineTools(EngineTools&) = delete;

		explicit EngineTools(EngineTools&&) = delete;

		EngineTools& operator=(EngineTools&) = delete;

		EngineTools& operator=(EngineTools&&) = delete;

		~EngineTools() = default;

		void Initialize(GLFWwindow* glfwWindow, VkInstance instance, VkDescriptorPool descriptorPool, VkRenderPass renderPass) const;

		void RenderWindow(VkCommandBuffer cmdBuffer) const;

		void CleanUp() const;

		LOGICAL_DEVICE

	private:
		mutable bool showDemoWindow;

		static void CheckResult(VkResult result);

		void Begin() const;

		void EndFrame(VkCommandBuffer cmdBuffer) const;
	};
}
