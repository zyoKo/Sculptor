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

#include <vulkan/vulkan.h>

// The minwindef.h 'max' macro is colliding with limits 'max' function
#ifdef max
#undef max
#endif

#include <cstdint>
#include <limits>
#include <algorithm>
#include <array>