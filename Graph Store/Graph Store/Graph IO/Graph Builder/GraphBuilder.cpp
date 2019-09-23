#include "GraphBuilder.h"
#include "File Parser/Open File Fail Exception/OpenFileFailException.h"
#include "Graph IO/Exception.h"
#include "Graph Factory/GraphFactory.h"
#include "Graph IO/GraphIOConstants.h"
#include "Invocer/Invocer.h"

namespace GraphIO
{
	std::unique_ptr<Graph>
	GraphBuilder::buildFromFile(const String& fileName)
	{
		auto resourceReleaser =
			Invocer{ [this]() noexcept { releaseResources(); } };
		openFile(fileName);
		tryToBuildAGraphFromOpenedFile(fileName);

		return std::move(graph);
	}

	void GraphBuilder::openFile(const String& name)
	{
		assert(!fileParser.hasOpenedFile());

		try
		{
			fileParser.openFile(name);
		}
		catch (OpenFileFailException& e)
		{
			throw Exception(String(e.what()));
		}
	}

	void GraphBuilder::tryToBuildAGraphFromOpenedFile(const String& fileName)
	{
		assert(fileParser.hasOpenedFile());

		try
		{
			buildAGraph();
		}
		catch (std::exception& e)
		{
			throw Exception(e.what() + "\nError in: "_s + fileName);
		}
	}

	void GraphBuilder::buildAGraph()
	{
		createEmptyGraph();
		addVerticesToTheCreatedGraph();
		addEdgesHavingAddedVertices();
	}

	void GraphBuilder::createEmptyGraph()
	{
		assert(graph == nullptr);

		auto id = fileParser.readAndTrimLine();
		auto type = fileParser.readAndTrimLine();

		graph = GraphFactory::instance().createGraph(type, id);
	}

	void GraphBuilder::addVerticesToTheCreatedGraph()
	{
		assert(graph != nullptr);
		assert(identifiers.isEmpty());

		auto identifiersCount = parseUnsignedAndSkipUntil('\n');
		identifiers.reserve(identifiersCount);

		for (auto i = 0u; i < identifiersCount; ++i)
		{
			identifiers.add(fileParser.readAndTrimLine());

			graph->addVertex(identifiers[i]);
		}
	}

	void GraphBuilder::addEdgesHavingAddedVertices()
	{
		assert(graph != nullptr);

		auto edgesCount = parseUnsignedAndSkipUntil('\n');

		for (auto i = 1u; i <= edgesCount; ++i)
		{
			auto e = parseEdge();
			addEdge(e);
		}
	}

	GraphBuilder::RawEdge GraphBuilder::parseEdge()
	{
		auto edge = RawEdge();

		fileParser.skipUntil(EDGE_START);
		edge.startIDIndex = parseUnsignedAndSkipUntil(EDGE_ATTRIBUTE_SEPARATOR);
		edge.endIDIndex = parseUnsignedAndSkipUntil(EDGE_ATTRIBUTE_SEPARATOR);
		edge.weight = parseUnsignedAndSkipUntil(EDGE_END);
		fileParser.skipUntil('\n');

		return edge;
	}

	unsigned GraphBuilder::parseUnsignedAndSkipUntil(char c)
	{
		auto result = fileParser.parseUnsigned();
		fileParser.skipUntil(c);

		return result;
	}

	void GraphBuilder::addEdge(const RawEdge& edge)
	{
		auto& startID = identifiers[edge.startIDIndex];
		auto& endID = identifiers[edge.endIDIndex];

		auto& start = graph->getVertexWithID(startID);
		auto& end = graph->getVertexWithID(endID);

		graph->addEdge(start, end, edge.weight);
	}

	void GraphBuilder::releaseResources()
	{
		graph = nullptr;
		identifiers.empty();
		fileParser.closeFile();
	}
}
