#ifndef __PRINT_GRAPH_COMMAND_HEADER_INCLUDED__
#define __PRINT_GRAPH_COMMAND_HEADER_INCLUDED__

#include "../Abstract class/Command.h"
#include "../String Reader/StringReader.h"

class PrintGraphCommand : public Command
{
public:
	PrintGraphCommand() = default;
	PrintGraphCommand(const PrintGraphCommand&) = delete;
	PrintGraphCommand& operator=(const PrintGraphCommand&) = delete;
	PrintGraphCommand(PrintGraphCommand&&) = delete;
	PrintGraphCommand& operator=(PrintGraphCommand&&) = delete;
	virtual ~PrintGraphCommand() = default;

	virtual void execute(args::Subparser& parser) override;

private:
	static void printGraph(const String& graphID);
	static void printEdgesLeaving(Vertex& vertex, Graph& graph);
	static void printEdge(Edge& edge);

private:
	void parseArguments(args::Subparser& parser);
	void setGraphID(args::Positional<String, StringReader>& graphID);

private:
	String graphID;
};

#endif //__PRINT_GRAPH_COMMAND_HEADER_INCLUDED__