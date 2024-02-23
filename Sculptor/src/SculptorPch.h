#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <initializer_list>
#include <optional>
#include <map>
#include <set>
#include <assert.h>

// Included with "vma/vk_mem_alloc.h"
// #include <windows.h>
// #include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

// The minwindef.h 'max' macro is colliding with limits 'max' function
#ifdef max
#undef max
#endif

#include <cstdint>
#include <limits>
#include <algorithm>
#include <array>
#include <fstream>
#include <format>
#include <chrono>

// Project Necessary Files
#include "Core/Core.h"
#include "Utilities/GetShared.h"