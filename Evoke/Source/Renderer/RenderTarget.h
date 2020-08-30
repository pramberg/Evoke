#pragma once
#include "Core/Core.h"
#include "Texture.h"

namespace Evoke
{
	struct RenderTargetDescription
	{
		ETextureFormat Format;
		u32 Width, Height;
	};

	class RenderTarget2D
	{
	public:
		virtual ~RenderTarget2D() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Resize(u32 inWidth, u32 inHeight) = 0;

		// #TEMP: This should be removed when converting ImGui to render using Evoke.
		virtual void* RawHandle(u64 inIndex) = 0;

		virtual i32 Width() const = 0;
		virtual i32 Height() const = 0;

		static TSharedPtr<RenderTarget2D> Create(u32 inWidth, u32 inHeight, const std::initializer_list<ETextureFormat>& inFormats);
	};
}