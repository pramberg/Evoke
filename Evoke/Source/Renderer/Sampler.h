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

	struct SamplerDescription
	{
		ETextureAddressMode AddressMode = ETextureAddressMode::Wrap;
		ETextureFilter TextureFilter = ETextureFilter::Anisotropic;
		u32 AnisotropicLevels = 1;
		glm::vec4 BorderColor{ 0.0f };
	};

	class Sampler
	{
	public:
		virtual ~Sampler() = default;

		virtual void UpdateDescription(const SamplerDescription& inDescription) = 0;

		virtual void Bind(u32 inSlot) = 0;
		virtual void Unbind(u32 inSlot) = 0;

		static TSharedPtr<Sampler> Create(const SamplerDescription& inDescription);
	};
}