#include "PCH.h"
#include "Outliner.h"
#include "ECS\Components.h"
#include "ECS\Scene.h"
#include "Core\Input.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Core\Application.h"

namespace Evoke
{
	Outliner::Outliner(const ScenePtr& inScene) : Widget("Outliner"), mScene(inScene.get())
	{
	}

	Outliner::Outliner() : Widget("Outliner"), mScene(nullptr)
	{
	}

	void Outliner::SetScene(const ScenePtr& inScene)
	{
		mScene = inScene.get();
	}

	void Outliner::Render()
	{
		if (!mScene)
		{
			mScene = Application::Get().MainScene();
		}

		entt::registry& registry = mScene->Registry();
		
		// This needs to happen before, so it can be replaced by the entity specific popup menu.
		RenderWindowPopupMenu();

		registry.each([&](entt::entity inEntity)
		{
			ImGui::PushID((i32)inEntity);
			NameComponent& nc = registry.get<NameComponent>(inEntity);
			SelectionComponent& sc = registry.get<SelectionComponent>(inEntity);

			if (registry.valid(mCurrentRenameEntity) && inEntity == mCurrentRenameEntity)
			{
				ImGui::SetKeyboardFocusHere();

				ImGui::SetNextItemWidth(-1);

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 6, 6 });
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
				ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 4, 4 });
				if (ImGui::InputText("##RenameWidget", &nc.Name, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					mCurrentRenameEntity = entt::null;
				
				ImGui::PopStyleVar(3);

				ImGui::SetItemDefaultFocus();
			}
			else if (ImGui::Selectable(fmt::format("{} - {}", nc.Name.c_str(), (i32)inEntity).c_str(), mScene->IsSelected(inEntity), ImGuiSelectableFlags_AllowDoubleClick))
			{
				if (ImGui::IsMouseDoubleClicked(0))
				{
					mCurrentRenameEntity = inEntity;
				}

				if (Input::IsKeyPressed(EKeyCode::LeftControl))
				{
					mScene->ToggleSelection(inEntity);
				}
				else if (Input::IsKeyPressed(EKeyCode::LeftShift))
				{
					mScene->ExtendSelection(inEntity);
				}
				else
				{
					mScene->SetSelection({inEntity});
				}
			}

			if (ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered())
			{
				mCurrentRenameEntity = entt::null;
				if (ImGui::IsWindowHovered())
				{
					mScene->SetSelection({});
				}
			}

			RenderEntityPopupMenu(inEntity, registry);

			ImGui::PopID();
		});
	}

	void Outliner::PreRender()
	{
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 1, 1 });
	}

	void Outliner::PostRender()
	{
		//ImGui::PopStyleVar();
	}

	void Outliner::RenderWindowPopupMenu()
	{
		if (ImGui::BeginPopupContextWindow("OutlinerWindowPopupMenu"))
		{
			if (ImGui::Button("Add Entity"))
			{
				Entity entity = mScene->CreateEntity();
				entity.AddComponent<StaticMeshComponent>();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	void Outliner::RenderEntityPopupMenu(entt::entity inEntity, entt::registry& inRegistry)
	{
		if (ImGui::BeginPopupContextItem("OutlinerEntityPopupMenu"))
		{
			if (ImGui::Button("Remove"))
			{
				inRegistry.destroy(inEntity);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

}