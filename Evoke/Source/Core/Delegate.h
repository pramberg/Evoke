#pragma once
#include "PCH.h"

using DelegateId = size_t;

/**
 * A single cast delegate.
 *
 * @tparam	ReturnType	The return type of the delegate.
 * @tparam	Parameters	The signature of the delegate.
 */
template<typename ReturnType, typename... Parameters>
class Delegate
{
public:
	using DelegateFn = std::function<ReturnType(Parameters...)>;

	Delegate() : mDelegate(nullptr) {}
	
	void Bind(DelegateFn inCallback) { mDelegate = inCallback; }
	b8 Unbind(DelegateFn inCallback) { mDelegate = nullptr; }

	template<typename... Arguments>
	ReturnType Execute(Arguments... inArguments)
	{
		if (mDelegate)
			return mDelegate(inArguments...);
		return (ReturnType)0;
	}

private:
	DelegateFn mDelegate;
};

/**
 * A delegate with multiple functions.
 *
 * @tparam	Parameters	The signature of the delegate.
 */
template<typename... Parameters>
class MulticastDelegate
{
public:
	using DelegateFn = std::function<void(Parameters...)>;

	MulticastDelegate() {}

	struct Handle
	{
		Handle(DelegateId inId, const DelegateFn& inFunction) : Id(inId), Function(inFunction) {}
		DelegateId Id;
		DelegateFn Function;
	};

	DelegateId Subscribe(const DelegateFn& inCallback)
	{
		mCallbacks.emplace_back(mCurrentId, inCallback);
		return mCurrentId++;
	}

	b8 Unsubscribe(DelegateId inId)
	{
		for (size_t i = 0; i < mCallbacks.size(); i++)
		{
			if (mCallbacks[i].Id == inId)
			{
				mCallbacks.erase(mCallbacks.begin() + i);
				return true;
			}
		}
		return false;
	}

	template<typename... Arguments>
	void Broadcast(Arguments... inArguments)
	{
		for (const auto& callback : mCallbacks)
		{
			callback.Function(inArguments...);
		}
	}

private:
	std::vector<Handle> mCallbacks;
	DelegateId mCurrentId = 0;
};

/**
 * A delegate whose bound functions can be prioritized and stop further
 * broadcasting based on the boolean value the function returns.
 *
 * @tparam	Parameters	The signature of the delegate.
 */
template<typename... Parameters>
class OrderedDelegate
{
public:
	using DelegateFn = std::function<bool(Parameters...)>;

	OrderedDelegate() {}

	struct Handle
	{
		Handle(DelegateId inId, const DelegateFn& inFunction, i32 inPriority) : Id(inId), Function(inFunction), Priority(inPriority){}
		DelegateId Id;
		DelegateFn Function;
		i32 Priority;
	};

	DelegateId Subscribe(const DelegateFn& inCallback, i32 inPriority = 0)
	{
		b8 emplaced = false;
		for (i32 i = 0; i < mCallbacks.size(); i++)
		{
			if (inPriority > mCallbacks[i].Priority)
			{
				mCallbacks.emplace(mCallbacks.begin() + i, mCurrentId, inCallback, inPriority);
				emplaced = true;
				break;
			}
		}
		if (!emplaced)
			mCallbacks.emplace_back(mCurrentId, inCallback, inPriority);

		return mCurrentId++;
	}

	b8 Unsubscribe(DelegateId inId)
	{
		for (size_t i = 0; i < mCallbacks.size(); i++)
		{
			if (mCallbacks[i].Id == inId)
			{
				mCallbacks.erase(mCallbacks.begin() + i);
				return true;
			}
		}
		return false;
	}

	template<typename... Arguments>
	void Broadcast(Arguments... inArguments)
	{
		for (const auto& callback : mCallbacks)
		{
			if (callback.Function(inArguments...))
				break;
		}
	}

private:
	std::vector<Handle> mCallbacks;
	DelegateId mCurrentId = 0;
};


#define EV_BIND_0(inFn) std::bind(&inFn, this)
#define EV_BIND_1(inFn) std::bind(&inFn, this, std::placeholders::_1)
#define EV_BIND_2(inFn) std::bind(&inFn, this, std::placeholders::_1, std::placeholders::_2)
#define EV_BIND_3(inFn) std::bind(&inFn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define EV_BIND_4(inFn) std::bind(&inFn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)