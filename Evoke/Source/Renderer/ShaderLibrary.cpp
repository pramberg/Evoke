#include "PCH.h"
#include "ShaderLibrary.h"
#include "Platform\Generic\Filesystem.h"

namespace Evoke
{
	ShaderLibrary::ShaderLibrary()
	{
		mFilewatcher.OnFileChanged.Subscribe(EV_BIND_2(ShaderLibrary::RecompileOnFileModified));
	}

	ShaderLibrary::ShaderLibrary(const std::initializer_list<std::tuple<StringView, const ShaderCompilerConfig&>> inShadersToLoad) : ShaderLibrary()
	{
		for (auto [shaderPath, config] : inShadersToLoad)
		{
			Load(shaderPath, config);
		}
	}

	TSharedPtr<Shader> ShaderLibrary::Load(StringView inFilepath, b8 inForceLoad)
	{
		return Load(inFilepath, ShaderCompilerConfig::BasicConfig());
	}

	TSharedPtr<Shader> ShaderLibrary::Load(StringView inFilepath, const ShaderCompilerConfig& inConfig, b8 inForceLoad /*= false*/)
	{
		auto absPath = std::filesystem::absolute(inFilepath);
		const String fullPath = absPath.string();

		if (Exists(fullPath))
			return mShaders[fullPath];

		absPath.remove_filename();
		mFilewatcher.AddPath(absPath.string() + "*.hlsl*");
		auto shader = Shader::Create(fullPath, inConfig);
		mShaders[fullPath] = shader;
		return shader;
	}

	b8 ShaderLibrary::Exists(StringView inFilepath) const
	{
		auto absPath = Filesystem::Absolute(inFilepath);
		return mShaders.find(absPath) != mShaders.end();
	}

	void ShaderLibrary::Update(f32 inDeltaTime)
	{
		while (!mRecompileQueue.Empty())
		{
			const string& filePath = mRecompileQueue.Front();
			if (Exists(filePath))	
				mShaders[filePath]->Recompile();
			mRecompileQueue.Pop();
		}
	}

	void ShaderLibrary::RecompileOnFileModified(const string& inFilepath, Filewatcher::EChangeType inChangeType)
	{
		if (Exists(inFilepath))
		{
			mRecompileQueue.Push(inFilepath);
			return;
		}

		b8 isIncludedInLoadedShader = false;
		// Check if the modified file is included in any loaded shaders
		for (auto [shaderPath, shader] : mShaders)
		{
			for (const string& includePath : shader->IncludedFiles())
			{
				if (inFilepath == includePath)
				{
					mRecompileQueue.Push(shaderPath);
					isIncludedInLoadedShader = true;
				}
			}
		}

		if (isIncludedInLoadedShader)
			EV_LOG(LogShader, ELogLevel::Trace, "Include file modified: {}", inFilepath);
	}
}