#pragma once
#include "Core/Core.h"

namespace Evoke
{
	enum class ShaderType : c8
	{
		Vertex,
		Hull,
		Domain,
		Geometry,
		Pixel,
		Compute,
		Count,
		None
	};

	class ShaderUtilities
	{
		static string GetNiceShaderTypeName(ShaderType inShaderType);
	};

	struct ShaderCompilerConfig
	{
		static const ShaderCompilerConfig& GetStandard()
		{
			static ShaderCompilerConfig config;
			config.EntryPoints["VSMain"] = ShaderType::Vertex;
			config.EntryPoints["PSMain"] = ShaderType::Pixel;
			return config;
		}

		void AddVertexShader(const string& inEntryPoint) { EntryPoints[inEntryPoint] = ShaderType::Vertex; }
		void AddPixelShader(const string& inEntryPoint) { EntryPoints[inEntryPoint] = ShaderType::Pixel; }

		std::unordered_map<string, ShaderType> EntryPoints;
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