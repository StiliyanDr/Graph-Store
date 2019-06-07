#include "GraphFilesFunctions.h"
#include "Graph/Abstract class/Graph.h"
#include "String/String.h"
#include "Graph IO/Exception.h"
#include "Graph IO/GraphIOConstants.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace GraphIO
{
	void tryToRemoveExistingFile(const fs::path& name);

	String getFileNameFor(const Graph& g)
	{
		return g.getID() + FILE_EXTENSION;
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