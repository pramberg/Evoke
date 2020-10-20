#pragma once
#include "Core/Core.h"

#include "StaticMeshLoader.h"
#include <entt/entt.hpp>

namespace Evoke
{
	class AssetRegistry
	{
	public:
		AssetRegistry();
		const std::vector<String> GetLoadedMeshIdentifiers() const { return mLoadedMeshes; }
		i32 GetLoadedMeshSelection(StringView inName) const;

		entt::resource_cache<StaticMesh>& GetResourceCache() { return mStaticMeshCache; }

	private:
		entt::resource_cache<StaticMesh> mStaticMeshCache{};
		std::vector<String> mLoadedMeshes;
	};
}