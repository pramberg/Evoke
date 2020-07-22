#include "PCH.h"
#include "ShaderLibrary.h"
#include "Platform\Generic\Filesystem.h"

namespace Evoke
{

	ShaderLibrary::ShaderLibrary(const std::initializer_list<string>& inPaths)
	{
		for (const string& path : inPaths)
		{
			auto absPath = std::filesystem::absolute(path);
			if (!absPath.extension().empty())
				absPath += Filesystem::Separator;
			mFilewatcher.AddPath(absPath.string() + "*.hlsl*");
		}
		mFilewatcher.OnFileChanged.Subscribe(EV_BIND_2(ShaderLibrary::RecompileOnFileModified));
	}

	TSharedPtr<Shader> ShaderLibrary::Load(const string& inFilepath, b8 inForceLoad /*= false*/)
	{
		return Load(inFilepath, ShaderCompilerConfig::BasicConfig());
	}

	TSharedPtr<Shader> ShaderLibrary::Load(const string& inFilepath, const ShaderCompilerConfig& inConfig, b8 inForceLoad /*= false*/)
	{
		auto absPath = Filesystem::Absolute(inFilepath);
		EV_CORE_ASSERT(!Exists(absPath), "Shader is already loaded.");
		auto shader = Shader::Create(absPath, inConfig);
		mShaders[absPath] = shader;
		return shader;
	}

	b8 ShaderLibrary::Exists(const string& inFilepath) const
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