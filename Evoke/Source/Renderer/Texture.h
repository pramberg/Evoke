#pragma once
#include "Core/Core.h"

namespace Evoke
{
	enum class ETextureAddressMode : c8
	{
		Wrap,
		Mirror,
		Clamp,
		Border,
	};

	enum class ETextureFilter : c8
	{
		Point,
		Linear,
		Anisotropic,
	};

	enum class ETextureType : c8
	{
		Texture1D,
		Texture2D,
		Texture3D,
		TextureCube,
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void SetBorderColor(glm::vec4 inColor) = 0;
		virtual glm::vec4 BorderColor() = 0;
		virtual void Bind(u32 inSlot) = 0;
	};

	class Texture1D : public Texture
	{

	};

	class Texture2D : public Texture
	{
	public:
		virtual void SetAddressMode(ETextureAddressMode inAddressMode) = 0;
		virtual void SetFilterMode(ETextureFilter inTextureFilter) = 0;
		virtual i32 Width() const = 0;
		virtual i32 Height() const = 0;

		static TSharedPtr<Texture2D> Create(StringView inPath);
	};

	class Texture3D : public Texture
	{

	};

	class TextureCube : public Texture
	{

	};
}