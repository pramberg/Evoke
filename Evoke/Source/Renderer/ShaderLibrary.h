#pragma once
#include "Core/Core.h"
#include "Platform/Generic/Filewatcher.h"
#include "Shader.h"
#include "Core/Queue.h"
#include "Core/Concurrency.h"

namespace Evoke
{
	class ShaderLibrary
	{
	public:
		ShaderLibrary(const std::initializer_list<string>& inPaths);
		TSharedPtr<Shader> Load(const string& inFilepath, b8 inForceLoad = false);
		TSharedPtr<Shader> Load(const string& inFilepath, const ShaderCompilerConfig& inConfig, b8 inForceLoad = false);
		b8 Exists(const string& inFilepath) const;
		void Update(f32 inDeltaTime);

	private:
		/** 
		 * Runs on the filewatcher thread. Adds relevant shaders to the recompile queue, so they can be recompiled and linked on the main thread.
		 * #TODO: Compile on multiple threads?
		 */
		void RecompileOnFileModified(const string& inFilepath, Filewatcher::EChangeType inChangeType);

	private:
		Filewatcher mFilewatcher;
		std::unordered_map<string, TSharedPtr<Shader>> mShaders;
		TQueue<string, EConcurrency::Asynchronous> mRecompileQueue;
	};
}