#include "DirectoryLoader.h"
#include <assert.h>
#include "../Graph IO/Graph Builder/Graph Builder Exception/GraphBuilderException.h"
#include "../Directory Files Iterator/Directory Files Iterator Exception/DirectoryFilesIteratorException.h"
#include "../Logger/Logger.h"

namespace GraphIO
{
	void DirectoryLoader::openDirectory(const String& path)
	{
		assert(!directoryIterator.isValid());

		try
		{
			directoryIterator.startIterationIn(path);
		}
		catch (DirectoryFilesIteratorException& e)
		{
			assert(!directoryIterator.isValid());
			Logger::logError(e);
		}
	}

	bool DirectoryLoader::thereAreFilesLeftToLoad() const
	{
		return directoryIterator.isValid();
	}

	std::unique_ptr<Graph> DirectoryLoader::loadCurrentFile()
	{
		assert(directoryIterator.isValid());

		try
		{
			return graphBuilder.buildFromFile(directoryIterator.getPathOfCurrentFile());
		}
		catch (GraphBuilderException& e)
		{
			Logger::logError(e);
			return nullptr;
		}
	}

	void DirectoryLoader::goToNextFile()
	{
		directoryIterator.advance();
	}
}
