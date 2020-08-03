#include "PCH.h"
#include "GraphicsContext.h"
#include "Renderer.h"
#include "Platform\OpenGL\OpenGLGraphicsContext.h"

namespace Evoke
{

	TUniquePtr<GraphicsContext> GraphicsContext::Create(void* inWindow)
	{
		switch (Renderer::API())
		{
		case ERenderAPI::OpenGL: return MakeUnique<OpenGLGraphicsContext>(inWindow);
		default:
			EV_CORE_ASSERT(false, "Unknown render API");
		}
		return nullptr;
	}

}