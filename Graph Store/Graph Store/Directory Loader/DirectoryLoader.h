#ifndef __DIRECTORY_LOADER_HEADER_INCLUDED__
#define __DIRECTORY_LOADER_HEADER_INCLUDED__

#include "../Directory Files Iterator/DirectoryFilesIterator.h"
#include "../Graph Builder/GraphBuilder.h"

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

#include "DirectoryLoader.hpp"

#endif //__DIRECTORY_LOADER_HEADER_INCLUDED__