#pragma once
#include "Core/Core.h"
#include "Widget.h"

namespace Evoke
{
	struct ConsoleLineData
	{
		ConsoleLineData(StringView inMessage, ELogLevel inLevel) : Message(inMessage), Level(inLevel) {}
		String Message;
		enum class ELogLevel Level;
	};

	class ConsoleWidget : public Widget
	{
	public:
		ConsoleWidget();
		virtual void Render() override;
		virtual void PreRender() override;
		virtual void PostRender() override;

		static std::vector<ConsoleLineData>& GetMessages() { return sMessages; }

	private:
		void RenderCriticalFrame();
		void RenderInput();
		void RenderFilter();
		b8 PassesFilter(const ConsoleLineData& inLineData);

		String mCommand;
		String mFilter;
		static std::vector<ConsoleLineData> sMessages;
	};
}