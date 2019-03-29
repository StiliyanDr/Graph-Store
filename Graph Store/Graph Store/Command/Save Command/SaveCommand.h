#ifndef __SAVE_COMMAND_HEADER_INCLUDED__
#define __SAVE_COMMAND_HEADER_INCLUDED__

#include "Command\Graph Command\GraphCommand.h"

class SaveCommand : public GraphCommand
{
public:
	SaveCommand() = default;

private:
	void parseArguments(args::Subparser& parser) override;
	void doExecute() override;
};

#endif //__SAVE_COMMAND_HEADER_INCLUDED__