#include "PCH.h"
#include "Sampler.h"
#include "Renderer.h"
#include "Platform\OpenGL\OpenGLSampler.h"

namespace Evoke
{
	TSharedPtr<Sampler> Sampler::Create(const SamplerDescription& inDescription)
	{
		switch (Renderer::API())
		{
		case ERenderAPI::OpenGL: return MakeShared<OpenGLSampler>(inDescription);
		default:
			EV_CORE_ASSERT(false, "Unknown render API.");
			return nullptr;
		}
	}
}