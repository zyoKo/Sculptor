#pragma once

#include "Logger/Assert.h"

namespace Sculptor
{
	template <typename T>
	class GetShared
	{
	public:
		GetShared(std::weak_ptr<T> weakPtr) noexcept
			:	weakReference(std::move(weakPtr))
		{ }

		bool operator==(std::nullptr_t)
		{
			const auto sharedPtr = weakReference.lock();

			return Validate(sharedPtr);
		}

		bool operator!=(std::nullptr_t)
		{
			const auto sharedPtr = weakReference.lock();

			return !Validate(sharedPtr);
		}

		std::shared_ptr<T> operator->()
		{
			auto sharedPtr = weakReference.lock();

			const bool isValid = Validate(sharedPtr);
			S_ASSERT(isValid == false, "Weak Pointer is invalid!")

			if (!isValid)
			{
				return nullptr;
			}

			return sharedPtr;
		}

		bool IsValid() noexcept
		{
			return !weakReference.expired();
		}

	private:
		std::weak_ptr<T> weakReference;

		static bool Validate(const std::shared_ptr<T>& sharedPtr)
		{
			if (!sharedPtr)
			{
				std::cerr << "Weak pointer is invalid or deleted.\n";

				return false;
			}

			return true;
		}
	};
}
