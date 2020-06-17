#pragma once
#include "Core/Core.h"

namespace Evoke
{
	enum class EShaderStage : c8
	{
		Vertex,
		Hull,
		Domain,
		Geometry,
		Pixel,
		Compute,
		Count
	};

	class ShaderUtilities
	{
		static string GetNiceShaderTypeName(EShaderStage inShaderType);
	};

	struct ShaderCompilerConfig
	{
		static const ShaderCompilerConfig& GetStandard()
		{
			static ShaderCompilerConfig config;
			config.EntryPoints["VSMain"] = EShaderStage::Vertex;
			config.EntryPoints["PSMain"] = EShaderStage::Pixel;
			return config;
		}

		void AddVertexShader(const string& inEntryPoint) { EntryPoints[inEntryPoint] = EShaderStage::Vertex; }
		void AddPixelShader(const string& inEntryPoint) { EntryPoints[inEntryPoint] = EShaderStage::Pixel; }

		std::unordered_map<string, EShaderStage> EntryPoints;
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	
		static std::shared_ptr<Shader> Create(const string& inFilepath, const ShaderCompilerConfig& inConfig = ShaderCompilerConfig::GetStandard());

	private:
	};
}