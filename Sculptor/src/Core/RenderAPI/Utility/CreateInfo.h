#pragma once

namespace Sculptor::Core
{
	template <typename T>
	T CreateInfo(T = {}, const void* pNextChain = VK_NULL_HANDLE);

	template <>
	[[nodiscard]] inline VkApplicationInfo CreateInfo(VkApplicationInfo info, const void* pNextChain /* = VK_NULL_HANDLE */)
	{
	    info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		info.pNext = pNextChain;
	    return info;
	}

	template <>
	[[nodiscard]] inline VkInstanceCreateInfo CreateInfo(VkInstanceCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		info.pNext = pNextChain;
		return info;
	}

	template <>
	[[nodiscard]] inline VkDescriptorSetAllocateInfo CreateInfo(VkDescriptorSetAllocateInfo info, const void* pNextChain /* = VK_NULL_HANDLE */)
	{
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.pNext = pNextChain;
		return info;
	}
}