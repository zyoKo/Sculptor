#include <SculptorPch.h>

#include "Utilities.h"

#include <stb_image.h>

#include "Core/Core.h"
#include "Logger/Assert.h"

namespace Sculptor
{
	std::vector<char> Utilities::ReadFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		S_ASSERT(!file.is_open(), "Failed to open file!");

		const auto fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), static_cast<std::streamsize>(fileSize));   // std::streamsize(long long)

		file.close();

		return buffer;
	}

	VkShaderModule Utilities::CreateShaderModule(const std::vector<char>& code, const VkDevice& device)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule{};

		VK_CHECK(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule), "Failed to create shader module.")

			return shaderModule;
	}

	stbi_uc* Utilities::LoadTexture(const std::string& fileName, VkDeviceSize& imageSize, int* textureWidth, int* textureHeight)
	{
		int textureChannel;

		stbi_uc* pixels = stbi_load(fileName.c_str(), textureWidth, textureHeight, &textureChannel, STBI_rgb_alpha);

		imageSize = static_cast<VkDeviceSize>(*textureWidth) * static_cast<VkDeviceSize>(*textureHeight) * 4;

		S_ASSERT(pixels == nullptr, "Failed to load texture image.")

		return pixels;
	}

	void Utilities::DestroyTexture(stbi_uc* texture)
	{
		stbi_image_free(texture);
	}
}
