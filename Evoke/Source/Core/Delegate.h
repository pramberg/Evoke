#pragma once
#include "PCH.h"

using DelegateId = size_t;

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