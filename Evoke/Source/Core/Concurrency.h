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
		// #Q: This fixes VS static analysis warning, but I don't want annotations everywhere to satisfy it. What to do?
		//_Acquires_lock_(mMutex)
		void Lock() { mMutex.lock(); }
		//_Releases_lock_(mMutex)
		void Unlock() { mMutex.unlock(); }
		std::mutex mMutex;
	};
}