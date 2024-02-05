#pragma once

namespace Sculptor
{
	template <typename T>
	class GetShared
	{
	public:
		GetShared(std::weak_ptr<T> weakPtr)
			:	weakReference(weakPtr)
		{ }

		std::shared_ptr<T> operator->()
		{
			const auto sharedPtr = weakReference.lock();

			const bool isValid = Validate(sharedPtr);

			if (!isValid)
			{
				return nullptr;
			}

			return sharedPtr;
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