#ifndef __LIST_GRAPHS_COMMAND_HEADER_INCLUDED__
#define __LIST_GRAPHS_COMMAND_HEADER_INCLUDED__

#include "../Abstract class/Command.h"

class ListGraphsCommand : public Command
{
public:
	ListGraphsCommand() = default;
	ListGraphsCommand(const ListGraphsCommand&) = delete;
	ListGraphsCommand& operator=(const ListGraphsCommand&) = delete;
	ListGraphsCommand(ListGraphsCommand&&) = delete;
	ListGraphsCommand& operator=(ListGraphsCommand&&) = delete;
	virtual ~ListGraphsCommand() = default;

	virtual void execute(args::Subparser& parser) override;

private:
	static void listIdentifiersOfGraphs();
};

#endif //__LIST_GRAPHS_COMMAND_HEADER_INCLUDED__