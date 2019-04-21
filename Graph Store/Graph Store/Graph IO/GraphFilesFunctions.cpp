#include "GraphFilesFunctions.h"
#include "Graph/Abstract class/Graph.h"
#include "String/String.h"
#include "Runtime Error/RuntimeError.h"
#include "Graph IO/GraphIOConstants.h"
#include <filesystem>

namespace GraphIO
{
	void tryToRemoveExistingFile(const std::filesystem::path& name);

	String getFileNameFor(const Graph& g)
	{
		return g.getID() + FILE_EXTENSION;
	}

	void removeFileFor(const Graph& g)
	{
		try
		{
			std::filesystem::path fileName =
				getFileNameFor(g).cString();

			if (std::filesystem::exists(fileName))
			{
				tryToRemoveExistingFile(fileName);
			}
		}
		catch (std::filesystem::filesystem_error& e)
		{
			throw RuntimeError(String(e.what()));
		}
	}

	void tryToRemoveExistingFile(const std::filesystem::path& name)
	{
		try
		{
			std::filesystem::remove(name);
		}
		catch (std::filesystem::filesystem_error&)
		{
			throw RuntimeError("Could not remove: " + name.string());
		}
	}

}