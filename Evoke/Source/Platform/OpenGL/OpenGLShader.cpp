#include "PCH.h"
#include "OpenGLShader.h"
#include "glad/glad.h"
#include "ShaderConductor/ShaderConductor.hpp"
#include <fstream>
#include <filesystem>

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

	static ShaderConductor::ShaderStage GetShaderConductorShaderStage(const EShaderStage& inShaderType)
	{
		switch (inShaderType)
		{
		case EShaderStage::Vertex:
			return ShaderConductor::ShaderStage::VertexShader;
		case EShaderStage::Hull:
			return ShaderConductor::ShaderStage::HullShader;
		case EShaderStage::Domain:
			return ShaderConductor::ShaderStage::DomainShader;
		case EShaderStage::Geometry:
			return ShaderConductor::ShaderStage::GeometryShader;
		case EShaderStage::Pixel:
			return ShaderConductor::ShaderStage::PixelShader;
		case EShaderStage::Compute:
			return ShaderConductor::ShaderStage::ComputeShader;
		default:
			EV_CORE_ASSERT(false, "Unknown shader stage");
			return ShaderConductor::ShaderStage::NumShaderStages;
		}
	}

	string LoadFile(const string& inFilepath)
	{
		std::ifstream filestream(inFilepath);
		std::stringstream buffer;
		buffer << filestream.rdbuf();
		return buffer.str();
	}

	string GetFilename(const string& inFilepath)
	{
		std::filesystem::path path(inFilepath);
		return path.filename().string();
	}

	OpenGLShader::OpenGLShader(const string& inFilepath, const ShaderCompilerConfig& inConfig)
	{
		// #TODO: Handle the case where the filepath is invalid.
		const string sourceData = LoadFile(inFilepath);
		const string fileName = GetFilename(inFilepath);

		b8 isValid = true;
		i32 success;
		std::vector<u32> createdShaders;

		const u32 program = glCreateProgram();

		for (auto& entryPointPair : inConfig.EntryPoints)
		{
			const string& entryPoint = entryPointPair.first;
			const EShaderStage shaderStage = entryPointPair.second;

			ShaderConductor::Compiler::SourceDesc sourceDesc;
			sourceDesc.entryPoint = entryPoint.c_str();
			sourceDesc.fileName = fileName.c_str();
			sourceDesc.source = sourceData.c_str();
			sourceDesc.stage = GetShaderConductorShaderStage(shaderStage);
			sourceDesc.defines = nullptr;
			sourceDesc.numDefines = 0;

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