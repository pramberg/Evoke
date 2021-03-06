#pragma once
#include "Core/Layer.h"
#include "Widgets/Widget.h"

namespace Evoke
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(b8 inDockable = true);
		~ImGuiLayer() = default;

		virtual void Attached() override;
		virtual void Detached() override;
		virtual void Update(f32 inDeltaTime, GraphicsContext* inOutGraphicsContext) override;

		void Begin();
		void End();

		MulticastDelegate<> OnRender;
	private:
		b8 mDockable;
		std::vector<TUniquePtr<Widget>> mWidgets;
		b8 mShowDemoWindow = false;

		struct Sel
		{
			Sel(StringView inName) : Name(inName) {}
			String Name;
			b8 Selected = false;
		};

		std::vector<Sel> names;
	};
}