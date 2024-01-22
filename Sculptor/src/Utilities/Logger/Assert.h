#pragma once

#ifdef DEBUG
	#define S_ASSERT(condition, message) \
		do { \
			if (condition) \
			{ \
				std::cerr << "Assertion[" #condition "] failed in [" << __FILE__ << "](" << __LINE__ << ") : " << message << std::endl; \
				__debugbreak(); \
			} \
		} while (false)

		#define SC_ASSERT(condition) \
		do { \
			if (condition) \
			{ \
				std::cerr << "Assertion[" #condition "] failed in [" << __FILE__ << "]:(" << __LINE__ << ")" << std::endl; \
				__debugbreak(); \
			} \
		} while (false)
#else
	#define S_ASSERT(condition, message)
	#define SC_ASSERT(condition)
#endif