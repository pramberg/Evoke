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

	struct ShaderCompilerConfig
	{
		struct EntryPoint
		{
			EntryPoint(const string& inName, EShaderStage inStage) : Name(inName), Stage(inStage) {}
			const string Name;
			const EShaderStage Stage;
		};

		struct Define
		{
			Define(const string& inName, const string& inValue) : Name(inName), Value(inValue) {}
			const string Name;
			const string Value;
		};

		static const ShaderCompilerConfig& GetStandard()
		{
			static ShaderCompilerConfig config;
			config.EntryPoints.emplace_back("VSMain", EShaderStage::Vertex);
			config.EntryPoints.emplace_back("PSMain", EShaderStage::Pixel);
			return config;
		}

		void AddVertexShader(const string& inEntryPoint) { EntryPoints.emplace_back(inEntryPoint, EShaderStage::Vertex); }
		void AddPixelShader(const string& inEntryPoint) { EntryPoints.emplace_back(inEntryPoint, EShaderStage::Pixel); }
		void AddDefine(const string& inName, const string& inValue) { Defines.emplace_back(inName, inValue); }

		std::vector<EntryPoint> EntryPoints;
		std::vector<Define> Defines;
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Recompile() = 0;
	
		static std::shared_ptr<Shader> Create(const string& inFilepath, const ShaderCompilerConfig& inConfig = ShaderCompilerConfig::GetStandard());

	private:
	};
}