#include "PCH.h"
#include "OpenGLTexture.h"
#include "OpenGLUtilities.h"
#include "Renderer\TextureUtilities.h"

#include <glad\glad.h>
#include <stb_image.h>

namespace Evoke
{
	OpenGLTexture2D::OpenGLTexture2D(StringView inFilePath, const TextureDescription& inDescription, const SamplerDescription& inSamplerDesc)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);

		i32 channels;
		f32* data = stbi_loadf(inFilePath.data(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);

		const auto [internalFormat, internalBaseFormat] = OpenGLUtilities::InternalTextureFormat(inDescription);
		const u32 numMips = TextureUtilities::CalculateNumMips(mWidth, mHeight);
		glTextureStorage2D(mTextureID, numMips, internalFormat, mWidth, mHeight);
		glTextureSubImage2D(mTextureID, 0, 0, 0, mWidth, mHeight, internalBaseFormat, GL_FLOAT, data);

		if (numMips > 1)
			glGenerateTextureMipmap(mTextureID);
		
		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(u32 inWidth, u32 inHeight, const TextureDescription& inDescription) : mWidth(inWidth), mHeight(inHeight)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);
		const auto [internalFormat, internalBaseFormat] = OpenGLUtilities::InternalTextureFormat(inDescription);
		const u32 numMips = TextureUtilities::CalculateNumMips(mWidth, mHeight);
		glTextureStorage2D(mTextureID, numMips, internalFormat, mWidth, mHeight);
		glTextureSubImage2D(mTextureID, 0, 0, 0, mWidth, mHeight, internalBaseFormat, GL_FLOAT, nullptr);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &mTextureID);
	}

	void* OpenGLTexture2D::RawHandle()
	{
		return reinterpret_cast<void*>(static_cast<u64>(mTextureID));
	}

	void OpenGLTexture2D::SetSampler(const TSharedPtr<Sampler>& inSampler)
	{
		mSampler = inSampler;
	}

	void OpenGLTexture2D::Bind(u32 inSlot)
	{
		glBindTextureUnit(inSlot, mTextureID);
		if (mSampler)
			mSampler->Bind(inSlot);
	}
}