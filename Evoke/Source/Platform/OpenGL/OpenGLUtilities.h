#pragma once
#include "Core/Core.h"
#include "Renderer/Sampler.h"
#include "Renderer/Texture.h"

#include <glad\glad.h>

namespace Evoke
{
	struct GLCombinedFormat
	{
		GLenum InternalFormat;
		GLenum BaseInternalFormat;
	};

	enum class GLAttachmentType : u32
	{
		Color = GL_COLOR_ATTACHMENT0,
		Depth = GL_DEPTH_ATTACHMENT,
		Stencil = GL_STENCIL_ATTACHMENT,
		DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
	};

	class OpenGLUtilities
	{
	public:
		static GLenum ConvertAddressMode(ETextureAddressMode inAddressMode);
		static GLenum ConvertMinFilter(ETextureFilter inFilter);
		static GLenum ConvertMagFilter(ETextureFilter inFilter);
		static GLCombinedFormat InternalTextureFormat(const TextureDescription& inDescription);
		static GLAttachmentType AttachmentType(ETextureFormat inTextureFormat);
	};
}