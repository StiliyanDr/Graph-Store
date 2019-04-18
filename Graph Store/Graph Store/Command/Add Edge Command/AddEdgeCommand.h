#ifndef __ADD_EDGE_COMMAND_HEADER_INCLUDED__
#define __ADD_EDGE_COMMAND_HEADER_INCLUDED__

#include "Command/Graph Command/GraphCommand.h"
#include "Graph/Abstract class/Graph.h"

class AddEdgeCommand : public GraphCommand
{
	using PositionalWeight = args::Positional<Graph::Edge::Weight>;

public:
	AddEdgeCommand() = default;

private:
	void parseArguments(args::Subparser& parser) override;
	void doExecute() override;

	void setStartVertexID(PositionalString& id);
	void setEndVertexID(PositionalString& id);
	void setWeight(PositionalWeight& w);

private:
	static const Graph::Edge::Weight DEFAULT_EDGE_WEIGHT = 1;

private:
	String startVertexID;
	String endVertexID;
	Graph::Edge::Weight weight;
};

#endif //__ADD_EDGE_COMMAND_HEADER_INCLUDED__