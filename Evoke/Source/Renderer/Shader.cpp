#include "PCH.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform\OpenGL\OpenGLShader.h"

namespace Evoke
{

	TSharedPtr<Shader> Shader::Create(const string& inFilepath, const ShaderCompilerConfig& inConfig)
	{
		switch (Renderer::GetAPI())
		{
		case ERenderAPI::OpenGL: return MakeShared<OpenGLShader>(inFilepath, inConfig);
		default:
			EV_CORE_ASSERT(false, "Unknown render API.");
			return nullptr;
		}
	}

}