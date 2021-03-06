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
		OpenGLShader(const String& inFilepath, const ShaderCompilerConfig& inConfig);
		~OpenGLShader();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Recompile() override;
		virtual const std::unordered_set<String>& IncludedFiles() const override;

	private:
		ShaderConductor::Blob* OnFileIncluded(const c8* inFilepath);

	private:
		std::filesystem::path mFilepath;
		ShaderCompilerConfig mConfig;
		u32 mRendererID;
		b8 mIsValid;
		std::unordered_set<String> mIncludedFiles;
	};
}