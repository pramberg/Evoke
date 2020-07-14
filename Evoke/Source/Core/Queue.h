#pragma once
#include "Core/Core.h"
#include "Concurrency.h"

#include <deque>

namespace Evoke
{
	// I have no idea if all of this is valid or not...
	template<typename T, EConcurrency TConcurrency = EConcurrency::Synchronous>
	class TQueue : public TOptionalConcurrency<TConcurrency>
	{
	public:
		using Container = std::deque<T>;
		using ValueType = typename Container::value_type;
		using ValueRef = typename Container::reference;
		using ConstValueRef = typename Container::const_reference;

		explicit TQueue() = default;
		explicit TQueue(std::initializer_list<T> inList) : mContainer(inList) {}
		explicit TQueue(TQueue<T>&& inQueue) : mContainer(std::move(inQueue.mContainer)) {}

		size_t Count() const { return mContainer.size(); }
		b8 Empty() const { return mContainer.empty(); }

		ValueRef Front() noexcept { return mContainer.front(); }
		ConstValueRef Front() const noexcept { return mContainer.front(); }
		ValueRef Back() noexcept { return mContainer.back(); }
		ConstValueRef Back() const noexcept { return mContainer.back(); }

		void Push(const ValueType& inValue) { Lock(); mContainer.push_back(inValue); Unlock(); }
		void Push(ValueType&& inValue) { Lock(); mContainer.push_back(std::move(inValue)); Unlock(); }

		template<typename... TValues>
		decltype(auto) Emplace(TValues&&... inValues) { Lock(); mContainer.emplace_back(std::forward<TValues>(inValues)...); Unlock(); }

		void Pop() { Lock(); mContainer.pop_front(); Unlock(); }
		void Swap(TQueue& inRight) { Lock(); inRight.Lock(); _Swap_adl(mContainer, inRight.mQueue); Unlock(); inRight.Unlock(); }

		typename Container::iterator begin() { return mContainer.begin(); }
		typename Container::iterator end() { return mContainer.end(); }
		typename Container::reverse_iterator rbegin() { return mContainer.rbegin(); }
		typename Container::reverse_iterator rend() { return mContainer.rend(); }

		typename Container::const_iterator begin() const { return mContainer.begin(); }
		typename Container::const_iterator end() const { return mContainer.end(); }
		typename Container::const_reverse_iterator rbegin() const { return mContainer.rbegin(); }
		typename Container::const_reverse_iterator rend() const { return mContainer.rend(); }

	private:
		Container mContainer{};
	};
}