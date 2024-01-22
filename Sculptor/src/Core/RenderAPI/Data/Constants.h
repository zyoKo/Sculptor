#pragma once

namespace Sculptor::Core
{
	struct APPLICATION
	{
		inline static const char* NAME = "Sculptor-Triangle";
		static constexpr uint32_t MAJOR = 1;
		static constexpr uint32_t MINOR = 0;
		static constexpr uint32_t PATCH = 0;
	};

	struct ENGINE
	{
		inline static const char* NAME = "Sculptor-Engine";
		static constexpr uint32_t MAJOR = 1;
		static constexpr uint32_t MINOR = 0;
		static constexpr uint32_t PATCH = 0;
	};

	struct API
	{
		inline static const char* NAME = "Vulkan";
		static constexpr uint32_t MAJOR = 1;
		static constexpr uint32_t MINOR = 3;
		static constexpr uint32_t PATCH = 261;
	};

	static const std::vector<const char*> VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };

	static const std::vector<const char*> DEVICE_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	// Graphics Pipeline
	static const std::vector<VkDynamicState> DYNAMIC_STATES = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	static constexpr float DEFAULT_LINE_WIDTH_FOR_RASTERIZER{ 1.0f };
}