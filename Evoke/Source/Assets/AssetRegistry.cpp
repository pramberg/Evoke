#include "PCH.h"
#include "AssetRegistry.h"
#include "Platform\Generic\Paths.h"
#include "Platform\Generic\Filesystem.h"

namespace Evoke
{
	AssetRegistry::AssetRegistry()
	{
		for (const auto& file : Filesystem::IterateDirectoryRecursive(Paths::ProjectPath()))
		{
			if (!file.is_regular_file())
				continue;

			if (Filesystem::Extension(file.path().string()) != ".fbx")
				continue;

			String projectRelative = Filesystem::Relative(file.path().string().c_str(), Paths::ProjectPath());
			entt::id_type id = entt::hashed_string::value(projectRelative.c_str());
			mStaticMeshCache.load<StaticMeshLoader>(id, file.path().string());
			mLoadedMeshes.emplace_back(std::move(projectRelative));
		}
	}

	i32 AssetRegistry::GetLoadedMeshSelection(StringView inName) const
	{
		auto meshIdentifiers = GetLoadedMeshIdentifiers();
		for (i32 i = 0; i < meshIdentifiers.size(); i++)
		{
			if (meshIdentifiers[i] == inName)
			{
				return i;
			}
		}
		return -1;
	}

}