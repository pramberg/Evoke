#include "PCH.h"
#include "OpenGLUtilities.h"

namespace Evoke
{

	GLenum OpenGLUtilities::ConvertAddressMode(ETextureAddressMode inAddressMode)
	{
		switch (inAddressMode)
		{
		case ETextureAddressMode::Wrap:
			return GL_REPEAT;
		case ETextureAddressMode::Mirror:
			return GL_MIRROR_CLAMP_TO_EDGE;
		case ETextureAddressMode::Clamp:
			return GL_CLAMP;
		case ETextureAddressMode::Border:
			return GL_CLAMP_TO_BORDER;
		}
		return GL_REPEAT;

	}

	GLenum OpenGLUtilities::ConvertMinFilter(ETextureFilter inFilter)
	{
		switch (inFilter)
		{
		case ETextureFilter::Point:
			return GL_NEAREST_MIPMAP_NEAREST;
		case ETextureFilter::Linear:
		case ETextureFilter::Anisotropic:
		default:
			return GL_LINEAR_MIPMAP_LINEAR;
			return GL_LINEAR_MIPMAP_LINEAR;
		}
	}

	GLenum OpenGLUtilities::ConvertMagFilter(ETextureFilter inFilter)
	{
		switch (inFilter)
		{
		case ETextureFilter::Point:
			return GL_NEAREST;
		case ETextureFilter::Linear:
		case ETextureFilter::Anisotropic:
		default:
			return GL_LINEAR;
		}
	}

	GLCombinedFormat OpenGLUtilities::InternalTextureFormat(const TextureDescription& inDescription)
	{
		switch (inDescription.Format)
		{
		case ETextureFormat::R32G32B32A32_Float:
			return { GL_RGBA32F, GL_RGBA };
		case ETextureFormat::R32G32B32A32_UINT:
			return { GL_RGBA32UI, GL_RGBA };
		case ETextureFormat::R32G32B32A32_SINT:
			return { GL_RGBA32I, GL_RGBA };
		case ETextureFormat::R32G32B32_Float:
			return { GL_RGB32F, GL_RGB };
		case ETextureFormat::R32G32B32_UINT:
			return { GL_RGB32UI, GL_RGB };
		case ETextureFormat::R32G32B32_SINT:
			return { GL_RGB32I, GL_RGB };
		case ETextureFormat::R16G16B16A16_Float:
			return { GL_RGBA16F, GL_RGBA };
		case ETextureFormat::R16G16B16A16_UNORM:
			return { GL_RGBA16, GL_RGBA };
		case ETextureFormat::R16G16B16A16_UINT:
			return { GL_RGBA16UI, GL_RGBA };
		case ETextureFormat::R16G16B16A16_SNORM:
			return { GL_RGBA16_SNORM, GL_RGBA };
		case ETextureFormat::R16G16B16A16_SINT:
			return { GL_RGBA16I, GL_RGBA };
		case ETextureFormat::R32G32_Float:
			return { GL_RG32F, GL_RG };
		case ETextureFormat::R32G32_UINT:
			return { GL_RG32UI, GL_RG };
		case ETextureFormat::R32G32_SINT:
			return { GL_RG32I, GL_RG };
		case ETextureFormat::D32_Float_S8X24_UINT:
			return { GL_DEPTH32F_STENCIL8, GL_RED };
		case ETextureFormat::X32_Typeless_G8X24_UINT:
			return { GL_DEPTH32F_STENCIL8, GL_RED };
		case ETextureFormat::R10G10B10A2_UNORM:
			return { GL_RGB10_A2, GL_RGBA };
		case ETextureFormat::R10G10B10A2_UINT:
			return { GL_RGB10_A2UI, GL_RGBA };
		case ETextureFormat::R11G11B10_Float:
			return { GL_R11F_G11F_B10F, GL_RGB };
		case ETextureFormat::R8G8B8A8_UNORM:
			return { GL_RGBA8, GL_RGBA };
		case ETextureFormat::R8G8B8A8_UNORM_sRGB:
			return { GL_SRGB8_ALPHA8, GL_RGBA };
		case ETextureFormat::R8G8B8A8_UINT:
			return { GL_RGBA8UI, GL_RGBA };
		case ETextureFormat::R8G8B8A8_SNORM:
			return { GL_RGBA8_SNORM, GL_RGBA };
		case ETextureFormat::R8G8B8A8_SINT:
			return { GL_RGBA8I, GL_RGBA };
		case ETextureFormat::R16G16_Float:
			return { GL_RG16F, GL_RG };
		case ETextureFormat::R16G16_UNORM:
			return { GL_RG16, GL_RG };
		case ETextureFormat::R16G16_UINT:
			return { GL_RG16UI, GL_RG };
		case ETextureFormat::R16G16_SNORM:
			return { GL_RG16_SNORM, GL_RG };
		case ETextureFormat::R16G16_SINT:
			return { GL_RG16I, GL_RG };
		case ETextureFormat::D32_Float:
			return { GL_DEPTH_COMPONENT32F, GL_RED };
		case ETextureFormat::R32_Float:
			return { GL_R32F, GL_RED };
		case ETextureFormat::R32_UINT:
			return { GL_R32UI, GL_RED };
		case ETextureFormat::R32_SINT:
			return { GL_R32I, GL_RED };
		case ETextureFormat::D24_UNORM_S8_UINT:
			return { GL_DEPTH24_STENCIL8, GL_RG };
		case ETextureFormat::R8G8_UNORM:
			return { GL_RG8, GL_RG };
		case ETextureFormat::R8G8_UINT:
			return { GL_RG8UI, GL_RG };
		case ETextureFormat::R8G8_SNORM:
			return { GL_RG8_SNORM, GL_RG };
		case ETextureFormat::R8G8_SINT:
			return { GL_RG8I, GL_RG };
		case ETextureFormat::R16_Float:
			return { GL_R16, GL_RED };
		case ETextureFormat::D16_UNORM:
			return { GL_DEPTH_COMPONENT16, GL_RED };
		case ETextureFormat::R16_UNORM:
			return { GL_R16, GL_RED };
		case ETextureFormat::R16_UINT:
			return { GL_R16UI, GL_RED };
		case ETextureFormat::R16_SNORM:
			return { GL_R16_SNORM, GL_RED };
		case ETextureFormat::R16_SINT:
			return { GL_R16I, GL_RED };
		case ETextureFormat::R8_UNORM:
			return { GL_R8, GL_RED };
		case ETextureFormat::R8_UINT:
			return { GL_R8UI, GL_RED };
		case ETextureFormat::R8_SNORM:
			return { GL_R8_SNORM, GL_RED };
		case ETextureFormat::R8_SINT:
			return { GL_R8I, GL_RED };
		case ETextureFormat::Unknown:
		default:
			return { GL_NONE, GL_NONE };
		}
	}

	GLAttachmentType OpenGLUtilities::AttachmentType(ETextureFormat inTextureFormat)
	{
		switch (inTextureFormat)
		{
		case ETextureFormat::R32G32B32A32_Float:
		case ETextureFormat::R32G32B32A32_UINT:
		case ETextureFormat::R32G32B32A32_SINT:
		case ETextureFormat::R32G32B32_Float:
		case ETextureFormat::R32G32B32_UINT:
		case ETextureFormat::R32G32B32_SINT:
		case ETextureFormat::R16G16B16A16_Float:
		case ETextureFormat::R16G16B16A16_UNORM:
		case ETextureFormat::R16G16B16A16_UINT:
		case ETextureFormat::R16G16B16A16_SNORM:
		case ETextureFormat::R16G16B16A16_SINT:
		case ETextureFormat::R32G32_Float:
		case ETextureFormat::R32G32_UINT:
		case ETextureFormat::R32G32_SINT:
		case ETextureFormat::R10G10B10A2_UNORM:
		case ETextureFormat::R10G10B10A2_UINT:
		case ETextureFormat::R11G11B10_Float:
		case ETextureFormat::R8G8B8A8_UNORM:
		case ETextureFormat::R8G8B8A8_UNORM_sRGB:
		case ETextureFormat::R8G8B8A8_UINT:
		case ETextureFormat::R8G8B8A8_SNORM:
		case ETextureFormat::R8G8B8A8_SINT:
		case ETextureFormat::R16G16_Float:
		case ETextureFormat::R16G16_UNORM:
		case ETextureFormat::R16G16_UINT:
		case ETextureFormat::R16G16_SNORM:
		case ETextureFormat::R16G16_SINT:
		case ETextureFormat::R32_Float:
		case ETextureFormat::R32_UINT:
		case ETextureFormat::R32_SINT:
		case ETextureFormat::R8G8_UNORM:
		case ETextureFormat::R8G8_UINT:
		case ETextureFormat::R8G8_SNORM:
		case ETextureFormat::R8G8_SINT:
		case ETextureFormat::R16_Float:
		case ETextureFormat::R16_UNORM:
		case ETextureFormat::R16_UINT:
		case ETextureFormat::R16_SNORM:
		case ETextureFormat::R16_SINT:
		case ETextureFormat::R8_UNORM:
		case ETextureFormat::R8_UINT:
		case ETextureFormat::R8_SNORM:
		case ETextureFormat::R8_SINT:
			return GLAttachmentType::Color;
		case ETextureFormat::D32_Float:
		case ETextureFormat::D16_UNORM:
			return GLAttachmentType::Depth;
		case ETextureFormat::D32_Float_S8X24_UINT:
		case ETextureFormat::X32_Typeless_G8X24_UINT:
		case ETextureFormat::D24_UNORM_S8_UINT:
			return GLAttachmentType::DepthStencil;
		case ETextureFormat::Unknown:
		default:
			return GLAttachmentType::Color;
		}
	}

}