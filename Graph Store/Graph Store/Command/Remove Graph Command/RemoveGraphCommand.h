#ifndef __REMOVE_GRAPH_COMMAND_HEADER_INCLUDED__
#define __REMOVE_GRAPH_COMMAND_HEADER_INCLUDED__

#include "Command\Graph Command\GraphCommand.h"
#include "String\String.h"

class RemoveGraphCommand : public GraphCommand
{
public:
	RemoveGraphCommand() = default;

private:
	void parseArguments(args::Subparser& parser) override;
	void doExecute() override;

private:
	String graphID;
};

#endif //__REMOVE_GRAPH_COMMAND_HEADER_INCLUDED__