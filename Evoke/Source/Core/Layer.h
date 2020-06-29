#pragma once
#include "Core.h"

namespace Evoke
{
	class Layer
	{
	public:
		Layer(const string& inName = "Layer");
		virtual ~Layer() = default;

		virtual void Attached() {}
		virtual void Detached() {}
		virtual void Update(f32 inDeltaTime) {}

		const string& GetName() const { return mName; }

	private:
		string mName;
	};
}