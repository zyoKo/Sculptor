#pragma once

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
