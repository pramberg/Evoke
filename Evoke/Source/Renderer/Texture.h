#pragma once
#include "Core/Core.h"
#include "Sampler.h"

namespace Evoke
{
	enum class ETextureCubeSource : c8
	{
		Cubemap,
		Panorama,
		Spherical,
	};

	enum class ETextureFormat
	{
		Unknown,
		R32G32B32A32_Float,
		R32G32B32A32_UINT,
		R32G32B32A32_SINT,
		R32G32B32_Float,
		R32G32B32_UINT,
		R32G32B32_SINT,
		R16G16B16A16_Float,
		R16G16B16A16_UNORM,
		R16G16B16A16_UINT,
		R16G16B16A16_SNORM,
		R16G16B16A16_SINT,
		R32G32_Float,
		R32G32_UINT,
		R32G32_SINT,
		D32_Float_S8X24_UINT,
		X32_Typeless_G8X24_UINT,
		R10G10B10A2_UNORM,
		R10G10B10A2_UINT,
		R11G11B10_Float,
		R8G8B8A8_UNORM,
		R8G8B8A8_UNORM_sRGB,
		R8G8B8A8_UINT,
		R8G8B8A8_SNORM,
		R8G8B8A8_SINT,
		R16G16_Float,
		R16G16_UNORM,
		R16G16_UINT,
		R16G16_SNORM,
		R16G16_SINT,
		D32_Float,
		R32_Float,
		R32_UINT,
		R32_SINT,
		D24_UNORM_S8_UINT,
		R8G8_UNORM,
		R8G8_UINT,
		R8G8_SNORM,
		R8G8_SINT,
		R16_Float,
		D16_UNORM,
		R16_UNORM,
		R16_UINT,
		R16_SNORM,
		R16_SINT,
		R8_UNORM,
		R8_UINT,
		R8_SNORM,
		R8_SINT,
	};

	struct TextureDescription
	{
		ETextureFormat Format = ETextureFormat::R8G8B8A8_UNORM;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(u32 inSlot) = 0;
		virtual void* RawHandle() = 0;
		virtual void SetSampler(const TSharedPtr<Sampler>& inSampler) = 0;
	};

	class Texture1D : public Texture
	{

	};

	class Texture2D : public Texture
	{
	public:
		virtual i32 Width() const = 0;
		virtual i32 Height() const = 0;

		static TSharedPtr<Texture2D> Create(StringView inPath, const TextureDescription& inDescription = TextureDescription(), const SamplerDescription& inSamplerDesc = SamplerDescription());
	};

	class Texture3D : public Texture
	{

	};

	class TextureCube : public Texture
	{
		static TSharedPtr<TextureCube> Create(StringView inPath, ETextureCubeSource inSource);
	};
}