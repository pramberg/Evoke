#include "PCH.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform\OpenGL\OpenGLShader.h"
#include <memory>

namespace Evoke
{

	std::shared_ptr<Shader> Shader::Create(const string& inFilepath, const ShaderCompilerConfig& inConfig)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::OpenGL: return std::make_shared<OpenGLShader>(inFilepath, inConfig);
		default:
			EV_CORE_ASSERT(false, "Unknown render API.");
			return nullptr;
		}
	}

}