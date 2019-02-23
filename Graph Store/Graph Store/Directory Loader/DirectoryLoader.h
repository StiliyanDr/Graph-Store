#ifndef __DIRECTORY_LOADER_HEADER_INCLUDED__
#define __DIRECTORY_LOADER_HEADER_INCLUDED__

#include "../Directory Files Iterator/DirectoryFilesIterator.h"
#include "../Graph IO/Graph Builder/GraphBuilder.h"

namespace GraphIO
{
	class DirectoryLoader
	{
	public:
		DirectoryLoader() = default;
		DirectoryLoader(const DirectoryLoader&) = delete;
		DirectoryLoader& operator=(const DirectoryLoader&) = delete;
		DirectoryLoader(DirectoryLoader&&) = delete;
		DirectoryLoader& operator=(DirectoryLoader&&) = delete;
		~DirectoryLoader() = default;

		template <class Function>
		void loadApplyingFunctionToEachGraph(const String& path, const Function& function);

	private:
		void openDirectory(const String& path);
		bool thereAreFilesLeftToLoad() const;
		std::unique_ptr<Graph> loadCurrentFile();
		void goToNextFile();

	private:
		DirectoryFilesIterator directoryIterator;
		GraphBuilder graphBuilder;
	};

	template <class Function>
	void DirectoryLoader::loadApplyingFunctionToEachGraph(const String& path, const Function& function)
	{
		openDirectory(path);

		std::unique_ptr<Graph> graph;

		while (thereAreFilesLeftToLoad())
		{
			graph = loadCurrentFile();

			if (graph != nullptr)
			{
				function(std::move(graph));
			}

			goToNextFile();
		}
	}
}

#endif //__DIRECTORY_LOADER_HEADER_INCLUDED__