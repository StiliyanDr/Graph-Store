#include "GraphFilesFunctions.h"
#include "Graph/Abstract class/Graph.h"
#include "String/String.h"
#include "Graph Collection/GraphCollection.h"
#include "Graph IO/Graph Builder/GraphBuilder.h"
#include "Directory Iterator/DirectoryIterator.h"
#include "Directory Iterator/ExtensionFilter.h"
#include "Graph IO/Exception.h"
#include "Graph IO/GraphIOConstants.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace GraphIO
{
	void tryToRemoveExistingFile(const fs::path& name);

	GraphCollection loadDirectory(const String& path)
	{
		auto graphs = GraphCollection();
		auto builder = GraphBuilder();
		auto iterator =
			DirectoryIterator(path, ExtensionFilter(FILE_EXTENSION));

		forEach(iterator, [&builder, &graphs](const auto& pathName)
		{
			auto graph = builder.buildFromFile(pathName);

			graphs.add(std::move(graph));
		});

		return graphs;
	}

	void removeFileFor(const Graph& g)
	{
		try
		{
			auto fileName =
				fs::path(getFileNameFor(g).cString());

			if (fs::exists(fileName))
			{
				tryToRemoveExistingFile(fileName);
			}
		}
		catch (fs::filesystem_error& e)
		{
			throw Exception(String(e.what()));
		}
	}

	String getFileNameFor(const Graph& g)
	{
		return g.getID() + FILE_EXTENSION;
	}

	void tryToRemoveExistingFile(const fs::path& name)
	{
		try
		{
			fs::remove(name);
		}
		catch (fs::filesystem_error&)
		{
			throw Exception("Could not remove: " + name.string());
		}
	}

}