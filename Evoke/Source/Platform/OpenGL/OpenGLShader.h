#pragma once
#include "Renderer\Shader.h"

#include <filesystem>

namespace ShaderConductor
{
	class Blob;
}

namespace Evoke
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const string& inFilepath, const ShaderCompilerConfig& inConfig);
		~OpenGLShader();

		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		ShaderConductor::Blob* OnFileIncluded(const c8* inFilepath);

	private:
		std::filesystem::path mFilepath;
		u32 mRendererID;
		b8 mIsValid;
	};
}