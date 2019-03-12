#include "DirectoryLoader.h"
#include "../Graph Builder/Graph Builder Exception/GraphBuilderException.h"
#include "Logger/Logger.h"
#include "Graph Collection/GraphCollection.h"
#include "Directory Iterator/DirectoryIterator.h"

namespace GraphIO
{
	GraphCollection DirectoryLoader::load(const String& path)
	{
		GraphCollection graphs;
		DirectoryIterator iterator(path);

		forEach(iterator, [&](const String& pathName)
		{
			std::unique_ptr<Graph> graph = loadFile(pathName);

			if (graph != nullptr)
			{
				graphs.add(std::move(graph));
			}
		});

		return graphs;
	}

	std::unique_ptr<Graph> DirectoryLoader::loadFile(const String& pathName)
	{
		try
		{
			return graphBuilder.buildFromFile(pathName);
		}
		catch (GraphBuilderException& e)
		{
			Logger::logError(e);
			return nullptr;
		}
	}
}
