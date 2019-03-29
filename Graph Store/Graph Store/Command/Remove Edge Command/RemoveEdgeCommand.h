#ifndef __REMOVE_EDGE_COMMAND_HEADER_INCLUDED__
#define __REMOVE_EDGE_COMMAND_HEADER_INCLUDED__

#include "Command\Graph Command\GraphCommand.h"

class RemoveEdgeCommand : public GraphCommand
{
public:
	RemoveEdgeCommand() = default;

private:
	void parseArguments(args::Subparser& parser) override;
	void doExecute() override;

private:
	String startID;
	String endID;
};

#endif //__REMOVE_EDGE_COMMAND_HEADER_INCLUDED__