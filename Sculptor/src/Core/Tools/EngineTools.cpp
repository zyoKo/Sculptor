#include <SculptorPch.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_vulkan.h>
#include <ImGui/imconfig.h>

#include "EngineTools.h"

#include "Core/Data/Constants.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Sculptor::Core
{
	EngineTools::EngineTools(std::weak_ptr<LogicalDevice> device) noexcept
		:	logicalDevice(std::move(device)),
			showDemoWindow(true)
	{ }

	void EngineTools::Initialize(GLFWwindow* glfwWindow, VkInstance instance, VkDescriptorPool descriptorPool, VkRenderPass renderPass) const
	{
		GetShared logicalDevicePtr{ logicalDevice };
		const auto device = logicalDevicePtr->Get();

		GetShared physicalDevicePtr{ logicalDevicePtr->GetPhysicalDevice() };
		const auto physicalDevice = physicalDevicePtr->GetPrimaryDevice();

		const U32 graphicsFamily = logicalDevicePtr->GetQueueFamilies().GetQueueFamilyIndices().GetGraphicsFamily().value();
		const auto graphicsQueue = logicalDevicePtr->GetQueueFamilies().GetGraphicsQueue();

		auto* imGuiContext = ImGui::CreateContext();
		ImGui_ImplGlfw_InitForVulkan(glfwWindow, true);

		ImGui_ImplVulkan_InitInfo info{};
		info.Instance			= instance;
		info.DescriptorPool		= descriptorPool;
		info.RenderPass			= renderPass;
		info.Device				= device;
		info.QueueFamily		= graphicsFamily;
		info.Queue				= graphicsQueue;
		info.PhysicalDevice		= physicalDevice;
		info.PipelineCache		= VK_NULL_HANDLE;
		info.Allocator			= VK_NULL_HANDLE;
		info.ImageCount			= MAX_FRAMES_IN_FLIGHT;
		info.MinImageCount		= MAX_FRAMES_IN_FLIGHT;
		info.MSAASamples		= VK_SAMPLE_COUNT_1_BIT;
		info.CheckVkResultFn	= CheckResult;
		ImGui_ImplVulkan_Init(&info);

		ImGui_ImplVulkan_CreateFontsTexture();

		vkDeviceWaitIdle(device);
	}

	void EngineTools::Begin() const
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EngineTools::RenderWindow(VkCommandBuffer cmdBuffer) const
	{
		Begin();

		if (showDemoWindow)
		{
			ImGui::ShowDemoWindow(&showDemoWindow);
		}

		EndFrame(cmdBuffer);
	}

	void EngineTools::EndFrame(VkCommandBuffer cmdBuffer) const
	{
		ImGui::Render();

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmdBuffer);
	}

	void EngineTools::CleanUp() const
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EngineTools::CheckResult(VkResult result)
	{
		S_ASSERT(result != VK_SUCCESS, "ImGui Failed while init.")
	}
}
