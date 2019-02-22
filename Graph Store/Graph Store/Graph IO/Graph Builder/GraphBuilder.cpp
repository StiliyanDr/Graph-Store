#include "GraphBuilder.h"
#include "../../File Parser/Open File Fail Exception/OpenFileFailException.h"
#include "Graph Builder Exception\GraphBuilderException.h"
#include "../../Graph Factory/GraphFactory.h"
#include "../GraphIOConstants.h"

namespace GraphIO
{
	std::unique_ptr<Graph>
		GraphBuilder::buildFromFile(const String& fileName)
	{
		openFile(fileName);
		tryToBuildAGraphFromOpenedFile(fileName);
		releaseResources();

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
			throw GraphBuilderException(String(e.what()));
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
			handleExceptionDuringBuilding(fileName, e);
		}
	}

	void GraphBuilder::handleExceptionDuringBuilding(const String& fileName,
		const std::exception& e)
	{
		graph = nullptr;
		releaseResources();

		throw GraphBuilderException(e.what() + "\nError in: "_s + fileName);
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

		String id = fileParser.readLine();
		String type = fileParser.readLine();

		graph = GraphFactory::instance().createGraph(type, id);
	}

	void GraphBuilder::addVerticesToTheCreatedGraph()
	{
		assert(graph != nullptr);
		assert(identifiers.isEmpty());

		unsigned identifiersCount = parseUnsignedAndSkipUntil('\n');
		identifiers.ensureSize(identifiersCount);

		for (unsigned i = 0; i < identifiersCount; ++i)
		{
			identifiers.add(fileParser.readLine());

			graph->addVertex(identifiers[i]);
		}
	}

	void GraphBuilder::addEdgesHavingAddedVertices()
	{
		assert(graph != nullptr);

		unsigned edgesCount = parseUnsignedAndSkipUntil('\n');

		for (unsigned i = 1; i <= edgesCount; ++i)
		{
			RawEdge e = parseEdge();
			addEdge(e);
		}
	}

	GraphBuilder::RawEdge GraphBuilder::parseEdge()
	{
		RawEdge edge;

		fileParser.skipUntil(EDGE_START);
		edge.startIDIndex = parseUnsignedAndSkipUntil(EDGE_ATTRIBUTE_SEPARATOR);
		edge.endIDIndex = parseUnsignedAndSkipUntil(EDGE_ATTRIBUTE_SEPARATOR);
		edge.weight = parseUnsignedAndSkipUntil(EDGE_END);
		fileParser.skipUntil('\n');

		return edge;
	}

	unsigned GraphBuilder::parseUnsignedAndSkipUntil(char symbol)
	{
		unsigned result = fileParser.parseUnsigned();
		fileParser.skipUntil(symbol);

		return result;
	}

	void GraphBuilder::addEdge(const RawEdge& edge)
	{
		String& startID = identifiers[edge.startIDIndex];
		String& endID = identifiers[edge.endIDIndex];

		Graph::Vertex& start = graph->getVertexWithID(startID);
		Graph::Vertex& end = graph->getVertexWithID(endID);

		graph->addEdge(start, end, edge.weight);
	}

	void GraphBuilder::releaseResources()
	{
		identifiers.empty();
		fileParser.closeFile();
	}
}
