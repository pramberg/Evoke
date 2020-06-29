#pragma once
#include "Core/Layer.h"

namespace Evoke
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(b8 inDockable = true);
		~ImGuiLayer() = default;

		virtual void Attached() override;
		virtual void Detached() override;
		virtual void Update(f32 inDeltaTime) override;

		void Begin();
		void End();
	private:
		b8 mDockable;
	};
}