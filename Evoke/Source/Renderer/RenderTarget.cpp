#include "PCH.h"
#include "RenderTarget.h"
#include "Renderer.h"

#include "Platform\OpenGL\OpenGLTexture.h"
#include "Platform\OpenGL\OpenGLRenderTarget.h"

namespace Evoke
{
	TSharedPtr<RenderTarget2D> RenderTarget2D::Create(u32 inWidth, u32 inHeight, const std::initializer_list<ETextureFormat>& inFormats)
	{
		switch (Renderer::API())
		{
		case ERenderAPI::OpenGL: return MakeShared<OpenGLRenderTarget2D>(inWidth, inHeight, inFormats);
		default:
			EV_CORE_ASSERT(false, "Unknown render API.");
			return nullptr;
		}
	}
}