#pragma once
#include "Core/Core.h"

#include "entt/entt.hpp"

namespace Evoke
{
	class Scene;

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity inHandle, Scene* inScene) : mEntityHandle(inHandle), mScene(inScene) {}
		Entity(const Entity& inOther) = default;

		template<typename T, typename... TArgs>
		decltype(auto) AddComponent(TArgs&&... inArgs)
		{
			EV_CORE_ASSERT(mScene, "Scene is null.");
			return mScene->mRegistry.emplace<T>(mEntityHandle, std::forward<TArgs>(inArgs)...);
		}

		template<typename T>
		decltype(auto) GetComponent()
		{
			EV_CORE_ASSERT(HasComponent<T>(), "Entity does not have component.");
			return mScene->mRegistry.get<T>(mEntityHandle);
		}

		template<typename T>
		b8 HasComponent()
		{
			EV_CORE_ASSERT(mScene, "Scene is null.");
			return mScene->mRegistry.has<T>(mEntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			EV_CORE_ASSERT(HasComponent<T>(), "Entity does not have component.");
			return mScene->mRegistry.remove<T>(mEntityHandle);
		}

	private:
		entt::entity mEntityHandle{ entt::null };
		Scene* mScene = nullptr;
	};
}