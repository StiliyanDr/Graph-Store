#include "GraphSaver.h"
#include "Graph IO/Exception.h"
#include "Graph IO/GraphIOConstants.h"
#include "Graph IO/GraphFilesFunctions.h"
#include "Invocer/Invocer.h"

namespace GraphIO
{
	void GraphSaver::save(const Graph& g)
	{
		auto resourceReleaser =
			Invocer{ [this]() noexcept { releaseResources(); } };
		decorateVerticesOf(g);
		openFileFor(g);
		saveDecoratedGraph(g);
	}

	void GraphSaver::decorateVerticesOf(const Graph& g)
	{
		assert(decoratedVertices.empty());
		assert(!file.is_open());

		decoratedVertices.reserve(g.getVerticesCount());

		auto iterator =
			g.getConstIteratorOfVertices();
		auto index = 0u;

		forEach(*iterator, [&](const auto& vertex)
		{
			decoratedVertices.emplace(vertex.getID(), index);
			++index;
		});
	}

	void GraphSaver::openFileFor(const Graph& g)
	{
		assert(!file.is_open());

		auto name = getFileNameFor(g);
		file.open(name.cString(), std::ios::out | std::ios::trunc);
		verifyFileIsOpen(name);
	}

	void GraphSaver::verifyFileIsOpen(const String& fileName) const
	{
		if (!file.is_open())
		{
			throw Exception("Could not open \"" + fileName + "\"!");
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

		auto iterator =
			g.getConstIteratorOfVertices();

		forEach(*iterator, [&](const auto& vertex)
		{
			file << vertex.getID() << '\n';
		});
	}

	void GraphSaver::saveEdgesOf(const Graph& g)
	{
		file << g.getEdgesCount() << '\n';

		auto iterator =
			g.getConstIteratorOfEdges();

		forEach(*iterator, [&](const auto& edge)
		{
			file << EDGE_START << getIndexOf(edge.getStart())
				 << EDGE_ATTRIBUTE_SEPARATOR << getIndexOf(edge.getEnd())
				 << EDGE_ATTRIBUTE_SEPARATOR << edge.getWeight()
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
