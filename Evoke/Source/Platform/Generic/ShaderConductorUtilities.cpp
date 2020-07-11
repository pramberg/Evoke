#include "PCH.h"
#include "ShaderConductorUtilities.h"

#include <regex>

namespace Evoke
{
	ShaderConductor::ShaderStage ShaderConductorUtilities::ConvertShaderStage(const EShaderStage& inShaderStage)
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

	const std::vector<ShaderConductor::MacroDefine> ShaderConductorUtilities::ConvertDefines(const std::vector<ShaderCompilerConfig::Define>& inDefines)
	{
		if (inDefines.empty())
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

	const ShaderConductorUtilities::ErrorDataContainer ShaderConductorUtilities::ParseErrorBlob(const ShaderConductor::Blob* inErrorBlob)
	{
		string data((c8*)inErrorBlob->Data());
		
		ErrorDataContainer errorData;

		// Matches an entire entry, and captures the relevant data
		std::regex regex{ R"((\w*\.*\w*):(\d+):(\d+): (\w+): ([\s\S]+?\^))" };
		std::smatch match;
		while (std::regex_search(data, match, regex))
		{
			const string fileName = match[1].str();
			const i32 line = std::stoi(match[2].str());
			const i32 column = std::stoi(match[3].str()); // #TODO: Use column too
			const string errorType = match[4].str();
			string info = match[5].str();
			info[0] = std::toupper(info[0]);

			if (errorType == "error")
			{
				errorData.Errors.emplace_back(fileName, line, info);
			}
			else if (errorType == "warning")
			{
				errorData.Warnings.emplace_back(fileName, line, info);
			}

			data = match.suffix();
		}

		return errorData;
	}

	const void ShaderConductorUtilities::LogDisassembly(const ShaderConductor::Compiler::ResultDesc& inResults, ShaderConductor::ShadingLanguage inLanguage)
	{
		ShaderConductor::Compiler::DisassembleDesc desc;
		desc.binary = (u8*)inResults.target->Data();
		desc.binarySize = inResults.target->Size();
		desc.language = inLanguage;

		auto results = ShaderConductor::Compiler::Disassemble(desc);
		EV_LOG(LogShader, EV_TRACE, "\n{}", (c8*)results.target->Data());
	}

}


