#pragma once
#include "Widget.h"

namespace Evoke
{
	class Viewport : public Widget
	{
	public:
		Viewport() : Widget("Viewport") { }
		Viewport(StringView inTitle) : Widget(inTitle) { }
		~Viewport();

		virtual void Render() override;
		virtual void PreRender() override;
		virtual void PostRender() override;
		virtual void Resized() override;
	};
}