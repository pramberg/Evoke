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
		Amplify,
		Mesh,
		Count
	};

	struct ShaderCompilerConfig
	{
		struct EntryPoint
		{
			EntryPoint(const String& inName, EShaderStage inStage) : Name(inName), Stage(inStage) {}
			const String Name;
			const EShaderStage Stage;
		};

		struct Define
		{
			Define(const String& inName, const String& inValue) : Name(inName), Value(inValue) {}
			const String Name;
			const String Value;
		};

		static const ShaderCompilerConfig& BasicConfig()
		{
			static const ShaderCompilerConfig config {
				{ 
					EntryPoint{"VSMain", EShaderStage::Vertex}, 
					EntryPoint{"PSMain", EShaderStage::Pixel}
				}
			};
			return config;
		}

		void AddVertexShader(const String& inEntryPoint) { EntryPoints.emplace_back(inEntryPoint, EShaderStage::Vertex); }
		void AddPixelShader(const String& inEntryPoint) { EntryPoints.emplace_back(inEntryPoint, EShaderStage::Pixel); }
		void AddDefine(const String& inName, const String& inValue) { Defines.emplace_back(inName, inValue); }

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

		/** Returns the included files in this shader, including any potential sub-included files. */
		virtual const std::unordered_set<String>& IncludedFiles() const = 0;
	
	private:
		static TSharedPtr<Shader> Create(const String& inFilepath, const ShaderCompilerConfig& inConfig = ShaderCompilerConfig::BasicConfig());

		friend class ShaderLibrary;
	private:
	};
}