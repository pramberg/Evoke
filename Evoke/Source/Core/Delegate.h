#pragma once
#include "PCH.h"

using DelegateId = i32;

template<typename ReturnType, typename... Arguments>
class Delegate
{
public:
	using DelegateFn = std::function<ReturnType(Arguments...)>;

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


template<typename... Arguments>
class MulticastDelegate
{
public:
	using DelegateFn = std::function<void(Arguments...)>;
	
	MulticastDelegate() : mCurrentId(0) {}

	DelegateId Subscribe(const DelegateFn& inCallback)
	{ 
		mCallbacks.emplace(std::pair(mCurrentId, inCallback));
		return mCurrentId++;
	}

	b8 Unsubscribe(DelegateId inId) 
	{ 
		if (mCallbacks.count(inId))
		{
			mCallbacks.erase(inId);
			return true;
		}
		return false;
	}

	template<typename... Arguments>
	void Broadcast(Arguments... inArguments)
	{
		for (const auto& callbackPair : mCallbacks)
		{
			callbackPair.second(inArguments...);
		}
	}

private:
	std::map<DelegateId, DelegateFn> mCallbacks;
	DelegateId mCurrentId;
};