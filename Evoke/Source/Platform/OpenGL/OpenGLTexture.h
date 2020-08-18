#pragma once
#include "Renderer/Texture.h"

namespace Evoke
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(StringView inFilePath);
		virtual ~OpenGLTexture2D();

		virtual void SetAddressMode(ETextureAddressMode inAddressMode) override;
		virtual void SetFilterMode(ETextureFilter inTextureFilter) override;
		virtual void Bind(u32 inSlot) override;

		virtual i32 Width() const override { return mWidth; }
		virtual i32 Height() const override { return mHeight; }
		virtual void SetBorderColor(glm::vec4 inColor) override;
		virtual glm::vec4 BorderColor() override;

	private:
		u32 mTextureID;
		i32 mWidth;
		i32 mHeight;
	};
}