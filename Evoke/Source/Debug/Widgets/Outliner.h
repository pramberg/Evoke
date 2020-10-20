#pragma once
#include "Core/Core.h"
#include "Widget.h"

#include <entt/entt.hpp>

namespace Evoke
{
	class Scene;

	class Outliner : public Widget
	{
	public:
		using ScenePtr = TSharedPtr<Scene>;

		Outliner();
		Outliner(const ScenePtr& inScene);
		void SetScene(const ScenePtr& inScene);

		virtual void Render() override;
		virtual void PreRender() override;
		virtual void PostRender() override;

	private:
		void RenderWindowPopupMenu();
		void RenderEntityPopupMenu(entt::entity inEntity, entt::registry& inRegistry);

		Scene* mScene;
		entt::entity mCurrentRenameEntity = entt::null;
	};
}