#include "DirectoryLoader.h"
#include "Graph Collection/GraphCollection.h"
#include "Directory Iterator/DirectoryIterator.h"
#include "Graph IO/GraphIOConstants.h"

namespace GraphIO
{
	GraphCollection DirectoryLoader::load(const String& path)
	{
		GraphCollection graphs;
		DirectoryIterator iterator(path, ExtensionFilter(FILE_EXTENSION));

		forEach(iterator, [&](const String& pathName)
		{
			GraphCollection::GraphPointer graph =
				graphBuilder.buildFromFile(pathName);
			
			graphs.add(std::move(graph));
		});

		return graphs;
	}
}
