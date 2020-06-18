#include "PCH.h"
#include "ShaderConductorUtilities.h"

namespace Evoke
{
	ShaderConductor::ShaderStage ShaderConductorUtilities::GetShaderStage(const EShaderStage& inShaderType)
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

	const ShaderConductor::MacroDefine* ShaderConductorUtilities::GetDefines(const std::vector<ShaderCompilerConfig::Define>& inDefines)
	{
		if (inDefines.size() <= 0)
		{
			return nullptr;
		}

		ShaderConductor::MacroDefine* outDefines = new ShaderConductor::MacroDefine[inDefines.size()];
		for (i32 i = 0; i < inDefines.size(); i++)
		{
			outDefines[i].name = inDefines[i].Name.c_str();
			outDefines[i].value = inDefines[i].Value.c_str();
		}
		return outDefines;
	}
}


