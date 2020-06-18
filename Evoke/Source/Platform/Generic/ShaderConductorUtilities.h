#pragma once
#include "Core/Core.h"
#include "Renderer/Shader.h"

#include "ShaderConductor/ShaderConductor.hpp"

namespace Evoke
{
	class ShaderConductorUtilities
	{
	public:
		static ShaderConductor::ShaderStage GetShaderStage(const EShaderStage& inShaderType);
		static const ShaderConductor::MacroDefine* GetDefines(const std::vector<ShaderCompilerConfig::Define>& inDefines);
	};
}