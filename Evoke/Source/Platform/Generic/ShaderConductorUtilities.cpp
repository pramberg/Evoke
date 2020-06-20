#include "PCH.h"
#include "ShaderConductorUtilities.h"

#include <regex>

namespace Evoke
{
	ShaderConductor::ShaderStage ShaderConductorUtilities::GetShaderStage(const EShaderStage& inShaderStage)
	{
		switch (inShaderStage)
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

	const std::vector<ShaderConductor::MacroDefine> ShaderConductorUtilities::GetDefines(const std::vector<ShaderCompilerConfig::Define>& inDefines)
	{
		if (inDefines.size() <= 0)
		{
			return std::vector<ShaderConductor::MacroDefine>();
		}

		std::vector<ShaderConductor::MacroDefine> outDefines;
		outDefines.resize(inDefines.size());

		for (i32 i = 0; i < inDefines.size(); i++)
		{
			outDefines[i].name = inDefines[i].Name.c_str();
			outDefines[i].value = inDefines[i].Value.c_str();
		}
		return outDefines;
	}

	const ShaderConductorUtilities::ErrorData ShaderConductorUtilities::ParseErrorBlob(const ShaderConductor::Blob* inErrorBlob)
	{
		string data((c8*)inErrorBlob->Data());
		
		ErrorData errorData;

		// Matches an entire entry, and captures the relevant data
		std::regex regex{ "\\w+.\\w+:(\\d+):(\\d+): (\\w+): ([\\s\\S]+?\\^)" };
		std::smatch match;
		while (std::regex_search(data, match, regex))
		{
			const i32 line = std::stoi(match[1].str());
			const i32 column = std::stoi(match[2].str()); // #TODO: Use column too
			const string errorType = match[3].str();
			string info = match[4].str();
			info[0] = std::toupper(info[0]);

			if (errorType == "error")
			{
				errorData.Errors.emplace_back(line, info);
			}
			else if (errorType == "warning")
			{
				errorData.Warnings.emplace_back(line, info);
			}

			data = match.suffix();
		}

		return errorData;
	}

}

