#include "PCH.h"
#include "OpenGLShader.h"
#include <atlbase.h>
#include "dxc/dxcapi.h"
#include "glad/glad.h"

#include <locale>
#include <codecvt>

// #TODO: Error handling
// #TODO: Figure out how to handle strings
#include "dxc/Support/dxcapi.use.h"
namespace Evoke
{
	static GLenum GetGLSLShaderType(const ShaderType& inShaderType)
	{
		switch (inShaderType)
		{
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;
		case ShaderType::Hull:
			return GL_TESS_CONTROL_SHADER;
		case ShaderType::Domain:
			return GL_TESS_EVALUATION_SHADER;
		case ShaderType::Geometry:
			return GL_GEOMETRY_SHADER;
		case ShaderType::Pixel:
			return GL_FRAGMENT_SHADER;
		case ShaderType::Compute:
			return GL_COMPUTE_SHADER;
		case ShaderType::Count:
		case ShaderType::None:
		default:
			EV_CORE_ASSERT(false, "Unknown shader type");
			return GL_NONE;
		}
	}

	static std::wstring GetTargetProfile(const ShaderType& inShaderType)
	{
		switch (inShaderType)
		{
		case ShaderType::Vertex:
			return L"vs_6_0";
		case ShaderType::Hull:
			return L"hs_6_0";
		case ShaderType::Domain:
			return L"ds_6_0";
		case ShaderType::Geometry:
			return L"gs_6_0";
		case ShaderType::Pixel:
			return L"ps_6_0";
		case ShaderType::Compute:
			return L"cs_6_0";
		case ShaderType::Count:
		case ShaderType::None:
		default:
			EV_CORE_ASSERT(false, "Unknown shader type");
			return L"";
		}
	}

	std::wstring ConvertToWString(const std::string& str)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	static dxc::DxcDllSupport gSupport;

	OpenGLShader::OpenGLShader(const string& inFilepath, const ShaderCompilerConfig& inConfig)
	{
		// Is this right? Want to avoid Windows specific code...
		/*gSupport.Initialize();

		IDxcUtils* x;
		gSupport.CreateInstance(CLSID_DxcUtils, &x);*/

		// Create all DxCompiler objects that are needed to compile the shader.
		CComPtr<IDxcUtils> dxcUtils;
		CComPtr<IDxcCompiler3> dxcCompiler;
		DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
		DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));

		CComPtr<IDxcIncludeHandler> dxcIncludeHandler;
		dxcUtils->CreateDefaultIncludeHandler(&dxcIncludeHandler);

		u32 program = glCreateProgram();
		std::vector<u32> shaders;
		b8 isValid = true;
		i32 success;

		const std::wstring fileName = ConvertToWString(inFilepath);

		CComPtr<IDxcBlobEncoding> shaderSourceBlob = nullptr;
		dxcUtils->LoadFile(fileName.c_str(), nullptr, &shaderSourceBlob);

		DxcBuffer sourceBuffer;
		sourceBuffer.Ptr = shaderSourceBlob->GetBufferPointer();
		sourceBuffer.Size = shaderSourceBlob->GetBufferSize();
		sourceBuffer.Encoding = DXC_CP_ACP;

		for (auto& entryPoint : inConfig.EntryPoints)
		{
			const std::wstring entryPointName = ConvertToWString(entryPoint.first);
			const std::wstring targetProfile = GetTargetProfile(entryPoint.second);

			LPCWSTR arguments[]
			{
				L"-E", entryPointName.c_str(), // Entry point.
				L"-spirv",                     // Output SPIR-V
				fileName.c_str(),              // Optional shader source file name for error reporting and for PIX shader source view. 
				L"-T", targetProfile.c_str()   // Target.
			};

			CComPtr<IDxcResult> compilationResults;
			dxcCompiler->Compile(&sourceBuffer, arguments, _countof(arguments), dxcIncludeHandler, IID_PPV_ARGS(&compilationResults));

			CComPtr<IDxcBlobUtf8> compilationErrors = nullptr;
			compilationResults->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&compilationErrors), nullptr);

			if (compilationErrors != nullptr && compilationErrors->GetStringLength() != 0)
			{
				EV_CORE_ERROR("{}", compilationErrors->GetStringPointer());
				isValid = false;
			}

			CComPtr<IDxcBlob> spirvShaderBlob = nullptr;
			compilationResults->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&spirvShaderBlob), nullptr);

			u32 shader = glCreateShader(GetGLSLShaderType(entryPoint.second));
			glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, spirvShaderBlob->GetBufferPointer(), spirvShaderBlob->GetBufferSize());
			glSpecializeShaderARB(shader, entryPoint.first.c_str(), 0, nullptr, nullptr); // Required
			glAttachShader(program, shader);

			// Push back before potentially breaking, because shader needs to be cleaned up regardless.
			shaders.push_back(shader);

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
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			EV_CORE_ERROR("Shader linking failure: {}", infoLog);
			isValid = false;
		}

		if (mIsValid)
		{
			glDeleteProgram(mRendererID);
		}

		mRendererID = program;
		mIsValid = isValid;

		for (i32 shader : shaders)
		{
			glDetachShader(mRendererID, shader);
			glDeleteShader(shader);
		}
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