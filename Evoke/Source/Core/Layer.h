#pragma once
#include "Core.h"
#include "Renderer\GraphicsContext.h"

namespace Evoke
{
	class Layer
	{
	public:
		Layer(const string& inName = "Layer");
		virtual ~Layer() = default;

		virtual void Attached() {}
		virtual void Detached() {}
		virtual void Update(EV_UNUSED f32 inDeltaTime, EV_UNUSED GraphicsContext* inOutGraphicsContext) {}

		const string& Name() const { return mName; }

	private:
		string mName;
	};
}