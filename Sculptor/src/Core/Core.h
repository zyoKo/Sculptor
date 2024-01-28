#pragma once

#include "Utilities/Logger/Assert.h"

#define VK_CHECK(condition, message, ...) \
	do \
	{ \
		S_ASSERT(condition != VK_SUCCESS, message, __VA_ARGS__) \
	} \
	while(false);

#define VK_CHECK_NM(condition) \
	do \
	{ \
		SC_ASSERT(condition != VK_SUCCESS) \
	} \
	while(false);

// TODO: Solve this macro problem later
//#define GET_3RD_ARG(arg1, arg2, arg3, ...) arg3
//#define VK_CHECK_MACRO_CHOOSER(...) \
//	GET_3RD_ARG(__VA_ARGS__, CHECK_M, CHECK)
//
//#define VK_CHECK(...) VK_CHECK_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)
