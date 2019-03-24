#include "DirectoryLoader.h"
#include "Graph Collection/GraphCollection.h"
#include "Directory Iterator/DirectoryIterator.h"

namespace GraphIO
{
	const String DirectoryLoader::EXTENSION = ".txt"_s;

	GraphCollection DirectoryLoader::load(const String& path)
	{
		GraphCollection graphs;
		DirectoryIterator iterator(path, ExtensionFilter(EXTENSION));

		forEach(iterator, [&](const String& pathName)
		{
			GraphCollection::GraphPointer graph =
				graphBuilder.buildFromFile(pathName);
			
			graphs.add(std::move(graph));
		});

		return graphs;
	}
}
