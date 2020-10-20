#include "PCH.h"
#include "ConsoleWidget.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Evoke
{
	std::vector<ConsoleLineData> ConsoleWidget::sMessages;

	ConsoleWidget::ConsoleWidget() : Widget("Console")
	{

	}

	void ConsoleWidget::Render()
	{
		if (ImGui::BeginPopupContextItem("Console Context Menu"))
		{
			if (ImGui::Button("Clear"))
			{
				sMessages.clear();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		RenderFilter();
		const ImGuiStyle& style = ImGui::GetStyle();
		auto region = ImGui::GetContentRegionAvail();
		ImGui::BeginChild("ConsoleMessageLayout", ImVec2(region.x, region.y - ImGui::GetFrameHeightWithSpacing()), false);

		ImGuiIO& io = ImGui::GetIO();
		ImGui::PushFont(io.Fonts->Fonts[2]); // #TODO: Don't access fonts this way.
		for (const ConsoleLineData& lineData : sMessages)
		{
			if (!PassesFilter(lineData))
				continue;

			ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
			switch (lineData.Level)
			{
			case ELogLevel::Info:
				color = { 0.066f, 0.631f, 0.054f, 1.0f };
				break;
			case ELogLevel::Warning:
				color = { 0.976f, 0.945f, 0.647f, 1.0f };
				break;
			case ELogLevel::Error:
				color = { 0.905f, 0.282f, 0.337f, 1.0f };
				break;
			case ELogLevel::Critical:
			{
				RenderCriticalFrame();
				break;
			}
			}
			ImGui::TextColored(color, "%s", lineData.Message.c_str());
		}
		ImGui::PopFont();

		// Auto-scroll
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
		ImGui::OpenPopupContextItem("Console Context Menu", 1);

		RenderInput();
	}

	void ConsoleWidget::PreRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 6, 2 });
	}

	void ConsoleWidget::PostRender()
	{
		ImGui::PopStyleVar();
	}

	void ConsoleWidget::RenderCriticalFrame()
	{
		// Critical should have a highlight, similar to the cmd console. This is a simplified version of ImGui::Selectable().
		const ImGuiStyle& style = ImGui::GetStyle();

		ImVec2 pos = ImGui::GetCursorScreenPos();
		pos.y += ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset;

		const float minX = pos.x;
		const float maxX = ImGui::GetContentRegionMaxAbs().x;

		ImVec2 minPos(minX, pos.y);
		ImVec2 maxPos(minX + maxX, pos.y + ImGui::GetTextLineHeight());

		const float spacing_x = style.ItemSpacing.x;
		const float spacing_y = style.ItemSpacing.y;
		const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
		const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
		minPos.x -= spacing_L;
		minPos.y -= spacing_U;
		maxPos.x += (spacing_x - spacing_L);
		maxPos.y += (spacing_y - spacing_U);

		u32 highlightColor = ImGui::GetColorU32({ 0.772f, 0.04f, 0.121f, 1.0f });
		ImGui::RenderFrame(minPos, maxPos, highlightColor, false, 0.0f);
	}

	void ConsoleWidget::RenderInput()
	{
		const ImGuiStyle& style = ImGui::GetStyle();
		auto size = ImGui::GetContentRegionAvail();
		ImGui::SetNextItemWidth(size.x - (ImGui::CalcTextSize("Command").x + style.ItemSpacing.x * 2.0f));
		ImGui::InputText("Command", &mCommand);
	}

	void ConsoleWidget::RenderFilter()
	{
		const ImGuiStyle& style = ImGui::GetStyle();
		auto size = ImGui::GetContentRegionAvail();
		ImGui::SetNextItemWidth(size.x - (ImGui::CalcTextSize("Filter").x + style.ItemSpacing.x * 2.0f));
		ImGui::InputText("Filter", &mFilter);
	}

	b8 ConsoleWidget::PassesFilter(const ConsoleLineData& inLineData)
	{
		if (!mFilter.empty())
		{
			if (mFilter == magic_enum::enum_name(inLineData.Level))
				return true;

			if (inLineData.Message.find(mFilter) == inLineData.Message.npos)
				return false;
		}
		return true;
	}

}