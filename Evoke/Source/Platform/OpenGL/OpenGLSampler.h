#pragma once
#include "Renderer/Sampler.h"

namespace Evoke
{
	class OpenGLSampler : public Sampler
	{
	public:
		OpenGLSampler();
		OpenGLSampler(const SamplerDescription& inDescription);
		virtual ~OpenGLSampler();

		virtual void UpdateDescription(const SamplerDescription& inDescription) override;

		virtual void Bind(u32 inSlot) override;
		virtual void Unbind(u32 inSlot) override;

	private:
		u32 mSamplerID;
	};
}