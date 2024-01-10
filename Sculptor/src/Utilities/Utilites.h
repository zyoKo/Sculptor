#pragma once
#include "Logger/Assert.h"

namespace Sculptor
{
    class Utilities
    {
    public:
        static std::vector<char> ReadFile(const std::string& filename)
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
    };
}
