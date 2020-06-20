#pragma once
#include "Core/Core.h"
#include "Renderer/Shader.h"

#include "ShaderConductor/ShaderConductor.hpp"

namespace Evoke
{
	class ShaderConductorUtilities
	{
	public:
		/** Holds error and warning information from an unsuccessful shader compilation. */
		struct ErrorData
		{
			ErrorData() = default;
			ErrorData(ErrorData&& inErrorData) noexcept
			{
				Errors = std::move(inErrorData.Errors);
				Warnings = std::move(inErrorData.Warnings);
			}

			std::vector<std::pair<i32, string>> Errors;
			std::vector<std::pair<i32, string>> Warnings;
		};

		/**
		 * Converts an Evoke shader stage into a ShaderConductor shader stage.
		 * @param 	inShaderType	Type of the in shader.
		 * @returns	The shader stage.
		 */
		static ShaderConductor::ShaderStage GetShaderStage(const EShaderStage& inShaderStage);

		/**
		 * Converts Evoke defines into a form that ShaderConductor can handle
		 * @param 	inDefines	The Evoke defines.
		 * @returns	The ShaderConductor defines.
		 */
		static const std::vector<ShaderConductor::MacroDefine> GetDefines(const std::vector<ShaderCompilerConfig::Define>& inDefines);

		/**
		 * Parses the error blob and tries to find errors and warnings so they can be printed out
		 * correctly.
		 * @param [in]	inErrorBlob	ShaderConductor's error blob from the result description.
		 * @returns	A struct that contains separated errors and warnings.
		 */
		static const ErrorData ParseErrorBlob(const ShaderConductor::Blob* inErrorBlob);

	};
}