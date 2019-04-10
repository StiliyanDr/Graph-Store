#ifndef __SEARCH_COMMAND_HEADER_INCLUDED__
#define __SEARCH_COMMAND_HEADER_INCLUDED__

#include "Command\Graph Command\GraphCommand.h"
#include "String\String.h"

class SearchCommand : public GraphCommand
{
public:
	SearchCommand() = default;

private:
	void parseArguments(args::Subparser& parser) override;
	void doExecute() override;

private:
	String sourceID;
	String targetID;
	String algorithmID;
};

#endif //__SEARCH_COMMAND_HEADER_INCLUDED__