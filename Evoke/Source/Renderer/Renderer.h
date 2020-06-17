#pragma once
#include "Core/Core.h"

namespace Evoke
{
	enum class RenderAPI : c8
	{
		OpenGL,
		Vulkan,
		DirectX11,
		DirectX12
	};

	class Renderer
	{
	public:
		static RenderAPI GetAPI() { return RenderAPI::OpenGL; }
	};
}