#include "PCH.h"
#include "StaticMeshLoader.h"
#include "Renderer\StaticMesh.h"

namespace Evoke
{
	TSharedPtr<StaticMesh> StaticMeshLoader::load(StringView inMesh) const
	{
		return MakeShared<StaticMesh>(inMesh);
	}

}