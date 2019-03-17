#ifndef __SAVE_COMMAND_HEADER_INCLUDED__
#define __SAVE_COMMAND_HEADER_INCLUDED__

#include "Command\Abstract Class\Command.h"

class SaveCommand : public Command
{
public:
	SaveCommand() = default;
	SaveCommand(const SaveCommand&) = delete;
	SaveCommand& operator=(const SaveCommand&) = delete;

	void execute(args::Subparser& parser) override;

private:
	static void saveGraphs();
};

#endif //__SAVE_COMMAND_HEADER_INCLUDED__