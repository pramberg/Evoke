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
			EV_LOG(LogEngine, EV_ERROR, "Can't find file \"{}\", included in \"{}\"", inFilepath, mFilepath.filename().string());

			// ShaderConductor crashes if we return nullptr here, so create an empty blob.
			return ShaderConductor::CreateBlob(nullptr, 0);
		}

		string fileSource = Filesystem::ReadFile(path.string());
		return ShaderConductor::CreateBlob(fileSource.c_str(), (u32)fileSource.size());
	}

	OpenGLShader::OpenGLShader(const string& inFilepath, const ShaderCompilerConfig& inConfig) : mFilepath(fs::absolute(inFilepath)), mConfig(inConfig), mRendererID(0), mIsValid(false)
	{
		Recompile();
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
	
	void OpenGLShader::Recompile()
	{
		// #TODO: Handle the case where the filepath is invalid.
		const string sourceData = Filesystem::ReadFile(mFilepath.string());
		const string fileName = mFilepath.filename().string();
		const auto defines = ShaderConductorUtilities::ConvertDefines(mConfig.Defines);

		b8 isValid = true;
		i32 success;
		std::vector<u32> createdShaders;

		const u32 program = glCreateProgram();

		// Setup ShaderConductor data
		ShaderConductor::Compiler::SourceDesc sourceDesc;
		sourceDesc.fileName = fileName.c_str();
		sourceDesc.source = sourceData.c_str();
		sourceDesc.defines = defines.data();
		sourceDesc.numDefines = (u32)defines.size();
		sourceDesc.loadIncludeCallback = EV_BIND_1(OpenGLShader::OnFileIncluded);

		ShaderConductor::Compiler::TargetDesc targetDesc;
		targetDesc.asModule = false;
		targetDesc.language = ShaderConductor::ShadingLanguage::SpirV;

		ShaderConductor::Compiler::Options options;

		for (auto [entryPoint, shaderStage] : mConfig.EntryPoints)
		{
			sourceDesc.entryPoint = entryPoint.c_str();
			sourceDesc.stage = ShaderConductorUtilities::ConvertShaderStage(shaderStage);

			ShaderConductor::Compiler::ResultDesc results = ShaderConductor::Compiler::Compile(sourceDesc, options, targetDesc);
			
			if (results.hasError && results.errorWarningMsg)
			{
				const auto errorData = ShaderConductorUtilities::ParseErrorBlob(results.errorWarningMsg);
				for (auto[sourceFileName, line, description] : errorData.Errors)
					EV_LOG_CUSTOM_LOCATION(LogShader, ELogLevel::Error, sourceFileName.c_str(), line, "{}", description);

				for (auto [sourceFileName, line, description] : errorData.Warnings)
					EV_LOG_CUSTOM_LOCATION(LogShader, ELogLevel::Warning, sourceFileName.c_str(), line, "{}", description);

				isValid = false;
				ShaderConductor::DestroyBlob(results.errorWarningMsg);
				ShaderConductor::DestroyBlob(results.target);
				break;
			}
			
			const u32 shader = glCreateShader(GetGLSLShaderStage(shaderStage));
			glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, results.target->Data(), results.target->Size());
			glSpecializeShaderARB(shader, entryPoint.c_str(), 0, nullptr, nullptr); // Required
			glAttachShader(program, shader);

			// Push back before potentially breaking, because shader needs to be cleaned up regardless.
			createdShaders.push_back(shader);

			ShaderConductor::DestroyBlob(results.errorWarningMsg);
			ShaderConductor::DestroyBlob(results.target);

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
			std::array<c8, 512> infoLog;
			glGetProgramInfoLog(program, (i32)infoLog.size(), nullptr, infoLog.data());
			EV_LOG(LogShader, EV_ERROR, "Linking failed: {}", infoLog.data());
			isValid = false;
		}

		if (isValid)
		{
			glDeleteProgram(mRendererID);
			mRendererID = program;
			EV_LOG(LogShader, ELogLevel::Trace, "Successfully compiled {}", fileName);
		}
		else
		{
			glDeleteProgram(program);
		}

		mIsValid = isValid;

		for (i32 shader : createdShaders)
		{
			glDetachShader(mRendererID, shader);
			glDeleteShader(shader);
		}
	}
}