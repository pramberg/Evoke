#include "PCH.h"
#include "Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer.h"

namespace Evoke
{

	TSharedPtr<Texture2D> Texture2D::Create(StringView inPath, const TextureDescription& inDescription, const SamplerDescription& inSamplerDesc)
	{
		switch (Renderer::API())
		{
		case ERenderAPI::OpenGL: return MakeShared<OpenGLTexture2D>(inPath, inDescription, inSamplerDesc);
		default:
			EV_CORE_ASSERT(false, "Unknown render API.");
			return nullptr;
		}
	}

}

