#include "PCH.h"
#include "DetailsWidget.h"
#include "imgui.h"
#include "Core\Application.h"
#include "ECS\Components.h"

namespace Evoke
{
	std::unordered_map<entt::id_type, TUniquePtr<DetailsComponentRenderer>> DetailsWidget::sFunctions;

	DetailsWidget::DetailsWidget() : Widget("Details")
	{
	}

	void DetailsWidget::RenderEntity(entt::registry& inRegistry, entt::entity inEntity)
	{
		ImGui::PushID((i32)inEntity);
		std::vector<DetailsComponentRenderer*> sortedFunctions;
		inRegistry.visit(inEntity, [&sortedFunctions](const entt::id_type inComponentType)
		{
			if (sFunctions.find(inComponentType) == sFunctions.end())
				return;

			auto& function = sFunctions.at(inComponentType);
			if (!function)
				return;

			sortedFunctions.push_back(function.get());
		});

		std::sort(sortedFunctions.begin(), sortedFunctions.end(), [](DetailsComponentRenderer* inA, DetailsComponentRenderer* inB)
		{
			return inA->GetPriority() > inB->GetPriority();
		});

		ImGui::Spacing();
		ImGui::PushStyleColor(ImGuiCol_Header, { 0.15f, 0.15f, 0.15f, 1.0f });

		for (auto* renderer : sortedFunctions)
			renderer->Render(inRegistry, inEntity);

		ImGui::PopStyleColor(1);
		ImGui::PopID();
	}

	void DetailsWidget::Render()
	{
		if (!mScene)
		{
			mScene = Application::Get().MainScene();
		}

		auto& registry = mScene->Registry();

		const auto selection = mScene->Selection();
		if (selection.empty())
		{
			return;
		}
		
		if (selection.size() == 1)
			RenderEntity(registry, selection[0]);
		else
		{
			ImGui::Spacing();
			ImGui::Separator();

			const i32 halfWindowSize = static_cast<i32>(ImGui::GetWindowSize().x * 0.5f); 
			ImGuiIO& io = ImGui::GetIO();
			ImFont* boldFont = io.Fonts->Fonts[1]; // #TODO: Make more reliable...
			for (entt::entity entity : selection)
			{
				const String entityName = fmt::format("{} - {}", registry.get<NameComponent>(entity).Name.c_str(), (i32)entity);
				auto textSize = ImGui::CalcTextSize(entityName.c_str());
				ImGui::SetCursorPosX(halfWindowSize - textSize.x * 0.5f);

				ImGui::PushFont(boldFont);
				ImGui::Text(entityName.c_str());
				ImGui::PopFont();

				ImGui::Separator();
				RenderEntity(registry, entity);
				ImGui::Separator();
			}
		}
	}

	void DetailsWidget::PreRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	}

	void DetailsWidget::PostRender()
	{
		ImGui::PopStyleVar();
	}

}

