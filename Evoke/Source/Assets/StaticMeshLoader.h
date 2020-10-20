#pragma once
#include "Core/Core.h"

#include <entt/entt.hpp>

namespace Evoke
{
	class StaticMesh;

	class StaticMeshLoader final : public entt::resource_loader<StaticMeshLoader, StaticMesh>
	{
	public:
		TSharedPtr<StaticMesh> load(StringView inMesh) const;
	};
}