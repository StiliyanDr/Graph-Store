#ifndef __USE_GRAPH_COMMAND_HEADER_INCLUDED__
#define __USE_GRAPH_COMMAND_HEADER_INCLUDED__

#include "Command/Graph Command/GraphCommand.h"
#include "String/String.h"

class UseGraphCommand : public GraphCommand
{
public:
	UseGraphCommand() = default;

private:
	void parseArguments(args::Subparser& parser) override;
	void doExecute() override;

private:
	String graphID;
};

#endif //__USE_GRAPH_COMMAND_HEADER_INCLUDED__