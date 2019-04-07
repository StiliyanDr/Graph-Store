#ifndef __ADD_GRAPH_COMMAND_HEADER_INCLUDED__
#define __ADD_GRAPH_COMMAND_HEADER_INCLUDED__

#include "Command\Graph Command\GraphCommand.h"

class AddGraphCommand : public GraphCommand
{
public:
	AddGraphCommand() = default;

private:
	static GraphCollection::GraphPointer tryToCreateGraph(const String& id,
		                                                  const String& type);

private:
	void parseArguments(args::Subparser& parser) override;
	void doExecute() override;
	void setGraphID(args::Positional<String, StringReader>& id);
	void setGraphType(args::Positional<String, StringReader>& type);

private:
	static const String DEFAULT_GRAPH_TYPE;

private:
	String graphID;
	String graphType;
};

#endif //__ADD_GRAPH_COMMAND_HEADER_INCLUDED__