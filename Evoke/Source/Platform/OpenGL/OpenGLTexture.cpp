#include "PCH.h"
#include "OpenGLTexture.h"

#include <glad\glad.h>
#include <stb_image.h>

namespace Evoke
{
	OpenGLTexture2D::OpenGLTexture2D(StringView inFilePath)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);

		i32 channels;
		u8* data = stbi_load(inFilePath.data(), &mWidth, &mHeight, &channels, 3);

		glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(mTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(mTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureStorage2D(mTextureID, 10, GL_RGB8, mWidth, mHeight);
		glTextureSubImage2D(mTextureID, 0, 0, 0, mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateTextureMipmap(mTextureID);
		
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &mTextureID);
	}

	void OpenGLTexture2D::SetAddressMode(ETextureAddressMode inAddressMode)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void OpenGLTexture2D::SetFilterMode(ETextureFilter inTextureFilter)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void OpenGLTexture2D::SetBorderColor(glm::vec4 inColor)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	glm::vec4 OpenGLTexture2D::BorderColor()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void OpenGLTexture2D::Bind(u32 inSlot)
	{
		glBindTextureUnit(inSlot, mTextureID);
	}

}