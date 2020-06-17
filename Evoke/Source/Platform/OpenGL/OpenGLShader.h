#pragma once
#include "Renderer\Shader.h"

namespace Evoke
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const string& inFilepath, const ShaderCompilerConfig& inConfig);

		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		//b8 Compile(const ShaderCompilerConfig& inConfig);

	private:
		u32 mRendererID;
		b8 mIsValid;
	};
}