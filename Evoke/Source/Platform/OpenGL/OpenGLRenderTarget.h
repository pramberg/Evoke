#pragma once
#include "Renderer/RenderTarget.h"

namespace Evoke
{
	class OpenGLRenderTarget2D : public RenderTarget2D
	{
	public:
		OpenGLRenderTarget2D(u32 inWidth, u32 inHeight, const std::initializer_list<ETextureFormat>& inDescriptions);

		virtual ~OpenGLRenderTarget2D();

		virtual void* RawHandle(u64 inIndex) override;
		virtual i32 Width() const override { return mWidth; }
		virtual i32 Height() const override { return mHeight; }

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Resize(u32 inWidth, u32 inHeight) override;

	private:
		void Invalidate();

		u32 mFramebufferID;
		std::vector<u32> mTextureIDs;
		std::vector<ETextureFormat> mFormats;
		i32 mWidth;
		i32 mHeight;
	};
}