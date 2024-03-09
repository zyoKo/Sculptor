#pragma once

namespace Sculptor
{
	using stbi_uc = unsigned char;

	class Utilities
	{
	public:
		static std::vector<char> ReadFile(const std::string& filename);

		static VkShaderModule CreateShaderModule(const std::vector<char>& code, const VkDevice& device);

		static stbi_uc* LoadTexture(const std::string& fileName, VkDeviceSize& imageSize, int* textureWidth, int* textureHeight);

		static void DestroyTexture(stbi_uc* texture);

		static std::string ExtractFileNameFromFilePath(std::string fileName);
	};
}
