#pragma once
#include "Core/Core.h"

namespace Evoke
{
	enum class ERenderAPI : c8
	{
		OpenGL,
		Vulkan,
		DirectX11,
		DirectX12,
		Metal
	};

	class Renderer
	{
	public:
		static ERenderAPI API() { return ERenderAPI::OpenGL; }
	};
}