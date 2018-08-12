#ifndef __COMMAND_HEADER_INCLUDED__
#define __COMMAND_HEADER_INCLUDED__

#include "../../../../../args-master/args-master/args.hxx"

class Command
{
public:
	virtual ~Command() = default;
	virtual void execute(args::Subparser& parser) = 0;

protected:
	Command() = default;
};

#endif //__COMMAND_HEADER_INCLUDED__