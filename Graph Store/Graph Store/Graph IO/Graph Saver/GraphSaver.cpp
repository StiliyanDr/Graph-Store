#include "GraphSaver.h"
#include "../../Runtime Error/RuntimeError.h"
#include "../GraphIOConstants.h"

namespace GraphIO
{
	void GraphSaver::save(const Graph& g)
	{
		try
		{
			decorateVerticesOf(g);
			openFileFor(g);
			saveDecoratedGraph(g);
			releaseResources();
		}
		catch (...)
		{
			releaseResources();
			throw;
		}
	}

	void GraphSaver::decorateVerticesOf(const Graph& g)
	{
		assert(decoratedVertices.empty());
		assert(!file.is_open());

		decoratedVertices.reserve(g.getVerticesCount());

		Graph::VerticesConstIterator iterator =
			g.getConstIteratorOfVertices();
		std::size_t index = 0;

		forEach(*iterator, [&](const Graph::Vertex& v)
		{
			decoratedVertices.emplace(v.getID(), index);
			++index;
		});
	}

	void GraphSaver::openFileFor(const Graph& g)
	{
		assert(!file.is_open());

		String name = getFileNameFor(g);
		file.open(name.cString(), std::ios::out | std::ios::trunc);
		verifyFileIsOpen(name);
	}

	String GraphSaver::getFileNameFor(const Graph& g)
	{
		return g.getID() + ".txt";
	}

	void GraphSaver::verifyFileIsOpen(const String& fileName)
	{
		if (!file.is_open())
		{
			throw RuntimeError("Could not open \"" + fileName + "\"!");
		}
	}

	void GraphSaver::saveDecoratedGraph(const Graph& g)
	{
		saveIDAndTypeOf(g);
		saveVerticesOf(g);
		saveEdgesOf(g);
	}

	void GraphSaver::saveIDAndTypeOf(const Graph& g)
	{
		file << g.getID() << '\n'
			<< g.getType() << '\n';
	}

	void GraphSaver::saveVerticesOf(const Graph& g)
	{
		file << g.getVerticesCount() << '\n';

		Graph::VerticesConstIterator iterator =
			g.getConstIteratorOfVertices();

		forEach(*iterator, [&](const Graph::Vertex& v)
		{
			file << v.getID() << '\n';
		});
	}

	void GraphSaver::saveEdgesOf(const Graph& g)
	{
		file << g.getEdgesCount() << '\n';

		Graph::EdgesConstIterator iterator =
			g.getConstIteratorOfEdges();

		forEach(*iterator, [&](const Graph::Edge& e)
		{
			file << EDGE_START << getIndexOf(e.getStart())
				 << EDGE_ATTRIBUTE_SEPARATOR << getIndexOf(e.getEnd())
				 << EDGE_ATTRIBUTE_SEPARATOR << e.getWeight()
				 << EDGE_END << '\n';
		});
	}

	std::size_t GraphSaver::getIndexOf(const Graph::Vertex& v) const
	{
		return decoratedVertices.at(v.getID());
	}

	void GraphSaver::releaseResources()
	{
		file.close();
		decoratedVertices.clear();
	}
}
