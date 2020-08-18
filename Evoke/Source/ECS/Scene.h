#pragma once
#include "Core/Core.h"
#include "Entity.h"
#include "entt/entt.hpp"

namespace Evoke
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(StringView inName = "New Entity");
		void Update(f32 inDeltaTime);

		void PilotEntity(Entity inEntity);
		void StopPilotingEntity();

	private:
		entt::registry mRegistry;
		Entity mMainCamera;
		Entity mActiveCamera;

		friend class Entity;
	};
}