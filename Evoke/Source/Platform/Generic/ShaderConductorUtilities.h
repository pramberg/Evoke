#pragma once
#include "Core/Core.h"
#include "Renderer/Shader.h"

#include "ShaderConductor/ShaderConductor.hpp"

namespace Evoke
{
	class ShaderConductorUtilities
	{
	public:
		struct ErrorData
		{
			ErrorData(const string& inFileName, i32 inLine, const string& inInfo) : FileName(inFileName), Line(inLine), Info(inInfo) {}
			string FileName;
			i32 Line;
			string Info;
		};

		/** Holds error and warning information from an unsuccessful shader compilation. */
		struct ErrorDataContainer
		{
			ErrorDataContainer() = default;
			ErrorDataContainer(ErrorDataContainer&& inErrorData) noexcept
			{
				Errors = std::move(inErrorData.Errors);
				Warnings = std::move(inErrorData.Warnings);
			}

			std::vector<ErrorData> Errors;
			std::vector<ErrorData> Warnings;
		};

		/**
		 * Converts an Evoke shader stage into a ShaderConductor shader stage.
		 * @param 	inShaderStage	The Evoke shader stage.
		 * @returns	The shader conductor shader stage.
		 */
		static ShaderConductor::ShaderStage ConvertShaderStage(const EShaderStage& inShaderStage);

		/**
		 * Converts Evoke defines into a form that ShaderConductor can handle
		 * @param 	inDefines	The Evoke defines.
		 * @returns	The ShaderConductor defines.
		 */
		static const std::vector<ShaderConductor::MacroDefine> ConvertDefines(const std::vector<ShaderCompilerConfig::Define>& inDefines);

		/**
		 * Parses the error blob and tries to find errors and warnings so they can be printed out
		 * correctly.
		 * @param [in]	inErrorBlob	ShaderConductor's error blob from the result description.
		 * @returns	A struct that contains separated errors and warnings.
		 */
		static const ErrorDataContainer ParseErrorBlob(const ShaderConductor::Blob* inErrorBlob);

		/**
		 * Prints the compiled and disassembled shader to the log.
		 * @param 	inResults 	The results struct from ShaderConductor's Compile().
		 * @param 	inLanguage	(Optional) The target language. Defaults to SPIR-V.
		 */
		static const void LogDisassembly(const ShaderConductor::Compiler::ResultDesc& inResults, ShaderConductor::ShadingLanguage inLanguage = ShaderConductor::ShadingLanguage::SpirV);
	};
}