#ifndef __LIST_GRAPHS_COMMAND_HEADER_INCLUDED__
#define __LIST_GRAPHS_COMMAND_HEADER_INCLUDED__

#include "../Abstract class/Command.h"

class ListGraphsCommand : public Command
{
public:
	ListGraphsCommand() = default;
	ListGraphsCommand(const ListGraphsCommand&) = default;
	ListGraphsCommand& operator=(const ListGraphsCommand&) = default;
	ListGraphsCommand(ListGraphsCommand&&) = default;
	ListGraphsCommand& operator=(ListGraphsCommand&&) = default;
	virtual ~ListGraphsCommand() = default;

	virtual void execute(args::Subparser& parser) override;

private:
	static void listIdentifiersOfGraphs();
};

#endif //__LIST_GRAPHS_COMMAND_HEADER_INCLUDED__