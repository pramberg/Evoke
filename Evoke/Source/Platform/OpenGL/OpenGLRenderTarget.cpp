#include "PCH.h"
#include "OpenGLRenderTarget.h"
#include "OpenGLUtilities.h"

#include <glad\glad.h>

namespace Evoke
{
	OpenGLRenderTarget2D::OpenGLRenderTarget2D(u32 inWidth, u32 inHeight, const std::initializer_list<ETextureFormat>& inDescriptions) : mWidth(inWidth), mHeight(inHeight)
	{
		glCreateFramebuffers(1, &mFramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);

		u32 count = 0;
		u32 colorCount = 0;
		mTextureIDs.resize(inDescriptions.size());
		mFormats.resize(inDescriptions.size());
		for (auto format : inDescriptions)
		{
			mFormats[count] = format;
			u32& tex = mTextureIDs[count++];
			glCreateTextures(GL_TEXTURE_2D, 1, &tex);
			const auto [internalFormat, internalBaseFormat] = OpenGLUtilities::InternalTextureFormat(TextureDescription{ format });
			glTextureStorage2D(tex, 1, internalFormat, mWidth, mHeight);

			auto attachmentType = OpenGLUtilities::AttachmentType(format);
			if (attachmentType == GLAttachmentType::Color)
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorCount++, GL_TEXTURE_2D, tex, 0);
			else
				glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attachmentType, GL_TEXTURE_2D, tex, 0);
		}
		EV_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLRenderTarget2D::~OpenGLRenderTarget2D()
	{
		for (u32& id : mTextureIDs)
		{
			glDeleteTextures(1, &id);
		}
		glDeleteFramebuffers(1, &mFramebufferID);
	}

	void OpenGLRenderTarget2D::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
		glViewport(0, 0, mWidth, mHeight);
	}

	void OpenGLRenderTarget2D::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLRenderTarget2D::Resize(u32 inWidth, u32 inHeight)
	{
		mWidth = inWidth;
		mHeight = inHeight;
		Invalidate();
	}

	void OpenGLRenderTarget2D::Invalidate()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);

		u32 count = 0;
		u32 colorCount = 0;
		for (u32& textureID : mTextureIDs)
		{
			auto format = mFormats[count++];
			glDeleteTextures(1, &textureID);

			glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
			const auto [internalFormat, internalBaseFormat] = OpenGLUtilities::InternalTextureFormat(TextureDescription{ format });
			glTextureStorage2D(textureID, 1, internalFormat, mWidth, mHeight);

			auto attachmentType = OpenGLUtilities::AttachmentType(format);
			if (attachmentType == GLAttachmentType::Color)
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorCount++, GL_TEXTURE_2D, textureID, 0);
			else
				glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attachmentType, GL_TEXTURE_2D, textureID, 0);
		}

		EV_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void* OpenGLRenderTarget2D::RawHandle(u64 inIndex)
	{
		return reinterpret_cast<void*>(static_cast<u64>(mTextureIDs[inIndex]));
	}
}