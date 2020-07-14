#pragma once
#include "Core/Core.h"
#include <type_traits>

namespace Evoke
{
	enum class EConcurrency
	{
		Synchronous,
		Asynchronous,
	};

	template<EConcurrency TConcurrency, typename = void>
	class TOptionalConcurrency
	{
	protected:
		void Lock() { }
		void Unlock() { }
	};

	template<EConcurrency TConcurrency>
	class TOptionalConcurrency<TConcurrency, std::enable_if_t<TConcurrency == EConcurrency::Asynchronous>>
	{
	protected:
		void Lock() { mMutex.lock(); }
		void Unlock() { mMutex.unlock(); }
		std::mutex mMutex;
	};
}