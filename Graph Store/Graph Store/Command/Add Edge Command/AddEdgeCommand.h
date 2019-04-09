#ifndef __ADD_EDGE_COMMAND_HEADER_INCLUDED__
#define __ADD_EDGE_COMMAND_HEADER_INCLUDED__

#include "Command\Graph Command\GraphCommand.h"
#include "Graph\Abstract class\Graph.h"

class AddEdgeCommand : public GraphCommand
{
public:
	AddEdgeCommand() = default;

private:
	void parseArguments(args::Subparser& parser) override;
	void doExecute() override;

	void setStartVertexID(args::Positional<String, StringReader>& id);
	void setEndVertexID(args::Positional<String, StringReader>& id);
	void setWeight(args::Positional<Graph::Edge::Weight>& w);

private:
	static const Graph::Edge::Weight DEFAULT_EDGE_WEIGHT = 1;

private:
	String startVertexID;
	String endVertexID;
	Graph::Edge::Weight weight;
};

#endif //__ADD_EDGE_COMMAND_HEADER_INCLUDED__