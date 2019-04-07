#ifndef __LIST_GRAPHS_COMMAND_HEADER_INCLUDED__
#define __LIST_GRAPHS_COMMAND_HEADER_INCLUDED__

#include "Command\Graph Command\GraphCommand.h"

class ListGraphsCommand : public GraphCommand
{
public:
	ListGraphsCommand() = default;

private:
	void parseArguments(args::Subparser& parser) override;
	void doExecute() override;
};

#endif //__LIST_GRAPHS_COMMAND_HEADER_INCLUDED__