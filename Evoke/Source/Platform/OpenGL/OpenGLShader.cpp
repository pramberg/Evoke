#include "PCH.h"
#include "OpenGLShader.h"
#include "Platform/Generic/Filesystem.h"
#include "Platform/Generic/ShaderConductorUtilities.h"

#include "glad/glad.h"
#include "ShaderConductor/ShaderConductor.hpp"

namespace fs = std::filesystem;

// #TODO: Error handling
namespace Evoke
{
	static GLenum GetGLSLShaderStage(const EShaderStage& inShaderType)
	{
		switch (inShaderType)
		{
		case EShaderStage::Vertex:
			return GL_VERTEX_SHADER;
		case EShaderStage::Hull:
			return GL_TESS_CONTROL_SHADER;
		case EShaderStage::Domain:
			return GL_TESS_EVALUATION_SHADER;
		case EShaderStage::Geometry:
			return GL_GEOMETRY_SHADER;
		case EShaderStage::Pixel:
			return GL_FRAGMENT_SHADER;
		case EShaderStage::Compute:
			return GL_COMPUTE_SHADER;
		default:
			EV_CORE_ASSERT(false, "Unknown shader stage");
			return GL_NONE;
		}
	}

	ShaderConductor::Blob* OpenGLShader::OnFileIncluded(const c8* inFilepath)
	{
		auto path = mFilepath;
		path.replace_filename(inFilepath);
		if (!fs::exists(path))
		{
			// #TODO: The "included in" part isn't necessarily helpful, what if it's a sub-include?
			EV_CORE_ERROR("Can't find file \"{}\", included in \"{}\"", inFilepath, mFilepath.filename().string());

			// ShaderConductor crashes if we return nullptr here, so create an empty blob.
			return ShaderConductor::CreateBlob(nullptr, 0);
		}

		string fileSource = Filesystem::ReadFile(path.string());
		return ShaderConductor::CreateBlob(fileSource.c_str(), (u32)fileSource.size());
	}

	OpenGLShader::OpenGLShader(const string& inFilepath, const ShaderCompilerConfig& inConfig) : mFilepath(fs::absolute(inFilepath)), mRendererID(0), mIsValid(false)
	{
		// #TODO: Handle the case where the filepath is invalid.
		const string sourceData = Filesystem::ReadFile(inFilepath);
		const string fileName = Filesystem::GetFilename(inFilepath);

		b8 isValid = true;
		i32 success;
		std::vector<u32> createdShaders;

		const u32 program = glCreateProgram();

		for (auto[entryPoint, shaderStage] : inConfig.EntryPoints)
		{
			ShaderConductor::Compiler::SourceDesc sourceDesc;
			sourceDesc.entryPoint = entryPoint.c_str();
			sourceDesc.fileName = fileName.c_str();
			sourceDesc.source = sourceData.c_str();
			sourceDesc.stage = ShaderConductorUtilities::GetShaderStage(shaderStage);
			sourceDesc.defines = ShaderConductorUtilities::GetDefines(inConfig.Defines);
			sourceDesc.numDefines = (u32)inConfig.Defines.size();
			sourceDesc.loadIncludeCallback = EV_BIND_1(OpenGLShader::OnFileIncluded);

			ShaderConductor::Compiler::TargetDesc targetDesc;
			targetDesc.asModule = false;
			targetDesc.language = ShaderConductor::ShadingLanguage::SpirV;

			ShaderConductor::Compiler::Options options;

			auto results = ShaderConductor::Compiler::Compile(sourceDesc, options, targetDesc);

			if (results.hasError && results.errorWarningMsg)
			{
				EV_CORE_ERROR("{}", (c8*)results.errorWarningMsg->Data());
				isValid = false;
				break;
			}

			const u32 shader = glCreateShader(GetGLSLShaderStage(shaderStage));
			glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, results.target->Data(), results.target->Size());
			glSpecializeShaderARB(shader, entryPoint.c_str(), 0, nullptr, nullptr); // Required
			glAttachShader(program, shader);

			// Push back before potentially breaking, because shader needs to be cleaned up regardless.
			createdShaders.push_back(shader);

			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				break;
			}
		}
		
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		// We don't really care about linking if compilation failed, since that's likely the issue
		if (!success && isValid)
		{
			c8 infoLog[512];
			glGetProgramInfoLog(program, sizeof(infoLog) / sizeof(infoLog[0]), NULL, infoLog);
			EV_CORE_ERROR("Failed to link shader: {}", infoLog);
			isValid = false;
		}

		if (mIsValid)
		{
			glDeleteProgram(mRendererID);
		}

		mRendererID = program;
		mIsValid = isValid;

		for (i32 shader : createdShaders)
		{
			glDetachShader(mRendererID, shader);
			glDeleteShader(shader);
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(mRendererID);
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(mRendererID);
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}
}