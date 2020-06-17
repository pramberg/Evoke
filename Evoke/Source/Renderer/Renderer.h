#pragma once
#include "Core/Core.h"

namespace Evoke
{
	enum class ERenderAPI : c8
	{
		OpenGL,
		Vulkan,
		DirectX11,
		DirectX12
	};

	class Renderer
	{
	public:
		static ERenderAPI GetAPI() { return ERenderAPI::OpenGL; }
	};
}