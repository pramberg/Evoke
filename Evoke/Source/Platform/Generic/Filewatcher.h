#pragma once
#include "Core/Core.h"
#include "Core/Delegate.h"

#include "efsw/efsw.hpp"

#include <mutex>

namespace Evoke
{
	class FilewatcherListener;

	class Filewatcher
	{
	public:
		enum class EChangeType
		{
			Created,
			Deleted,
			Modified,
			RenamedNewName,
			RenamedOldName
		};

		Filewatcher();
		Filewatcher(const string& inPath, b8 inRecursive = false);
		Filewatcher(const std::initializer_list<std::pair<string, b8>> inList);
		~Filewatcher();

		b8 AddPath(const string& inPath, b8 inRecursive = false);
		b8 RemovePath(const string& inPath);
		b8 MatchesPattern(const string& inPath) const;

		/**
		 * Event that gets broadcast when a file has changed.
		 * @param 	inWidth 	New window width.
		 * @param 	inHeight	New window height.
		 */
		MulticastDelegate<const string&, EChangeType> OnFileChanged;

		/**
		 * Event that gets broadcast when a file or directory gets renamed.
		 * @param 	inPath  		The path that changed.
		 * @param 	inChangeType	The type of change that happened.
		 */
		MulticastDelegate<const string&, const string&> OnRenamed;

		/**
		 * Event that gets broadcast when something changes in a watched directory.
		 * @param 	inPath  		The path that changed.
		 * @param 	inChangeType	The type of change that happened.
		 */
		MulticastDelegate<const string&, EChangeType> OnDirectoryChanged;

	private:
		TUniquePtr<efsw::FileWatcher> mEfswFilewatcher;
		TUniquePtr<FilewatcherListener> mListener;
		std::vector<string> mPatterns;
	};

	class FilewatcherListener : public efsw::FileWatchListener
	{
	public:
		FilewatcherListener() = default;
		FilewatcherListener(Filewatcher* inFilewatcher) : mFilewatcher(inFilewatcher) {}

		virtual void handleFileAction(efsw::WatchID inWatchId, const string& inDirectory, const string& inFilename, efsw::Action inAction, string inOldFilename) override
		{
			constexpr c8 separator = (c8)std::filesystem::path::preferred_separator;
			const string path = std::filesystem::absolute(inDirectory + separator + inFilename).string();
			switch (inAction)
			{
			case efsw::Action::Modified:
				if (mFilewatcher->MatchesPattern(path))
					mFilewatcher->OnFileChanged.Broadcast(path, Filewatcher::EChangeType::Modified);
				else
					mFilewatcher->OnDirectoryChanged.Broadcast(path, Filewatcher::EChangeType::Modified);
				break;
			case efsw::Action::Add:
				mFilewatcher->OnDirectoryChanged.Broadcast(path, Filewatcher::EChangeType::Created);
				break;
			case efsw::Action::Delete:
				mFilewatcher->OnDirectoryChanged.Broadcast(path, Filewatcher::EChangeType::Deleted);
				break;
			case efsw::Action::Moved:
				mFilewatcher->OnRenamed.Broadcast(path, std::filesystem::absolute(inDirectory + separator + inOldFilename).string());
				break;
			}
		}

	private:
		Filewatcher* mFilewatcher;
	};
}