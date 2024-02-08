#pragma once

using I16 = int16_t;
using I32 = int32_t;
using I64 = int64_t;

using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;

#define VK_CHECK(condition, message, ...) \
	do \
	{ \
		VkResult result = condition; \
		if (result != VK_SUCCESS) \
		{ \
			std::string formattedMessage = std::format("Condition \"{0}\" failed! ", #condition); \
			formattedMessage += std::format(message, __VA_ARGS__); \
			std::cerr << (formattedMessage); \
			__debugbreak(); \
		} \
	} \
	while(false);

#define VK_CHECK_NM(condition) \
	do \
	{ \
		std::string formattedMessage = std::format("Condition \"{0}\" failed! ", #condition); \
		std::cerr << (formattedMessage); \
		__debugbreak(); \
	} \
	while(false);
