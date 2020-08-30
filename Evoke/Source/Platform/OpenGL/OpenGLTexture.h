#pragma once
#include "Renderer/Texture.h"

namespace Evoke
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(StringView inFilePath, const TextureDescription& inDescription, const SamplerDescription& inSamplerDesc);
		OpenGLTexture2D(u32 inWidth, u32 inHeight, const TextureDescription& inDescription);

		virtual ~OpenGLTexture2D();

		virtual void Bind(u32 inSlot) override;
		virtual void* RawHandle() override;
		virtual void SetSampler(const TSharedPtr<Sampler>& inSampler) override;

		virtual i32 Width() const override { return mWidth; }
		virtual i32 Height() const override { return mHeight; }

	private:
		u32 mTextureID;
		i32 mWidth;
		i32 mHeight;
		TSharedPtr<Sampler> mSampler = nullptr;
	};
}