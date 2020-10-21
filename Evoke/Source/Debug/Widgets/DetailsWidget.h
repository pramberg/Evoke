#pragma once
#include "Core/Core.h"
#include "Widget.h"
#include "ECS/Scene.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include <boost/pfr.hpp>

namespace Evoke
{
	template<typename T>
	using DetailsComponentRendererFn = std::function<void(T&, entt::registry&, entt::entity)>;

	class DetailsComponentRenderer
	{
	public:
		DetailsComponentRenderer(StringView inName, i32 inPriority) : mName(inName), mPriority(inPriority) {}
		virtual void Render(entt::registry& inRegistry, entt::entity inEntity) = 0;
		i32 GetPriority() const { return mPriority; }

	protected:
		StringView mName = "Invalid";
		i8 mPriority = 0;
	};

	template<typename T>
	class TDetailsComponentRenderer : public DetailsComponentRenderer
	{
	public:
		TDetailsComponentRenderer(const DetailsComponentRendererFn<T>& inFunction, StringView inName, i32 inPriority) : DetailsComponentRenderer(inName, inPriority), mFn(inFunction) {}

		virtual void Render(entt::registry& inRegistry, entt::entity inEntity) override
		{
			if (!mFn || !inRegistry.has<T>(inEntity))
				return;
			
			auto& component = inRegistry.get<T>(inEntity);

			if (ImGui::CollapsingHeader(mName.data(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Spacing();

				ImGui::Indent();
				mFn(component, inRegistry, inEntity);
				ImGui::Unindent();

				ImGui::Spacing();
			}
			//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 1.0f, 1.0f });
			//ImGui::Separator();
			//ImGui::PopStyleVar();
		}
	private:
		DetailsComponentRendererFn<T> mFn;
	};

	template<typename T, b8... TArgs>
	constexpr auto GetImGuiDrawFunction()
	{
		static_assert(false, "Found no valid ImGui Draw Function for type!");
		return [](StringView inTitle, T& inValue) {};
	}

	template<> constexpr auto GetImGuiDrawFunction<f32>()       { return [](StringView inTitle, f32& inValue) { ImGui::DragFloat(inTitle.data(), &inValue); }; }
	template<> constexpr auto GetImGuiDrawFunction<i32>()       { return [](StringView inTitle, i32& inValue) { ImGui::DragInt(inTitle.data(), &inValue); }; }
	template<> constexpr auto GetImGuiDrawFunction<b8>()        { return [](StringView inTitle, b8& inValue) { ImGui::Checkbox(inTitle.data(), &inValue); }; }
	template<> constexpr auto GetImGuiDrawFunction<glm::vec2>() { return [](StringView inTitle, glm::vec2& inValue) { ImGui::DragFloat2(inTitle.data(), &inValue.x); }; }
	template<> constexpr auto GetImGuiDrawFunction<glm::vec3>() { return [](StringView inTitle, glm::vec3& inValue) { ImGui::DragFloat3(inTitle.data(), &inValue.x); }; }
	template<> constexpr auto GetImGuiDrawFunction<glm::vec4>() { return [](StringView inTitle, glm::vec4& inValue) { ImGui::DragFloat4(inTitle.data(), &inValue.x); }; }
	template<> constexpr auto GetImGuiDrawFunction<String>() { return [](StringView inTitle, String& inValue) { ImGui::InputText(inTitle.data(), &inValue); }; }
	template<> constexpr auto GetImGuiDrawFunction<String, false>() { return [](StringView inTitle, String& inValue) { ImGui::InputText(inTitle.data(), &inValue); }; }
	template<> constexpr auto GetImGuiDrawFunction<String, true>() { return [](StringView inTitle, String& inValue) { ImGui::InputTextMultiline(inTitle.data(), &inValue); }; }
	template<> constexpr auto GetImGuiDrawFunction<glm::mat4>()
	{ 
		return [](StringView inTitle, glm::mat4& inValue)
		{
			ImGui::Text("%s", inTitle.data());
			ImGui::DragFloat4("", &inValue[0].x);
			ImGui::DragFloat4("", &inValue[1].x);
			ImGui::DragFloat4("", &inValue[2].x);
			ImGui::DragFloat4("", &inValue[3].x);
		};
	}

	template<typename T>
	void RenderComponent(T& inOutComponent)
	{
		boost::pfr::for_each_field<T>(std::move(inOutComponent), [](auto& inOutField, i64 inIndex)
		{
			using CurrentType = typename std::decay_t<decltype(inOutField)>;
			GetImGuiDrawFunction<CurrentType>()(fmt::format("Item {}", inIndex), inOutField);
		});
	}

	class DetailsWidget : public Widget
	{
	public:
		DetailsWidget();

		virtual void Render() override;

		template<typename T>
		static void RegisterDetailPanelRenderer(const DetailsComponentRendererFn<T>& inFn, StringView inName, i32 inPriority = 0)
		{
			sFunctions[entt::type_info<T>::id()] = std::move(MakeUnique<TDetailsComponentRenderer<T>>(inFn, inName, inPriority));
		}

		template<typename T>
		static void RegisterDetailPanelRenderer(StringView inName, i32 inPriority = 0)
		{
			auto fn = [inName](T& inComponent, entt::registry& inRegistry, entt::entity inEntity)
			{
				RenderComponent(inComponent);
			};

			sFunctions[entt::type_info<T>::id()] = std::move(MakeUnique<TDetailsComponentRenderer<T>>(fn, inName, inPriority));
		}


		virtual void PreRender() override;
		virtual void PostRender() override;

		/*template<typename T>
		static void RegisterDetailPanelRenderer()
		{
			sFunctions[entt::type_info<T>::id()] = &T::Render;
		}*/
	private:
		void RenderEntity(entt::registry& inRegistry, entt::entity inEntity);

		Scene* mScene = nullptr;

		static std::unordered_map<entt::id_type, TUniquePtr<DetailsComponentRenderer>> sFunctions;
	};

#define REGISTER_DETAIL_PANEL_RENDERER(inComponent, inPriority) DetailsWidget::RegisterDetailPanelRenderer<inComponent>(#inComponent, inPriority)
#define REGISTER_DETAIL_PANEL_RENDERER_FN(inComponent, inPriority, inFn) DetailsWidget::RegisterDetailPanelRenderer<inComponent>(inFn, #inComponent, inPriority)
}