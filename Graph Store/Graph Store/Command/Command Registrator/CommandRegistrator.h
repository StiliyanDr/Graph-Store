#ifndef __COMMAND_REGISTRATOR_HEADER_INCLUDED__
#define __COMMAND_REGISTRATOR_HEADER_INCLUDED__

template <class ConcreteCommand>
class CommandRegistrator
{
public:
	CommandRegistrator(const char* name, const char* description);
	CommandRegistrator(const CommandRegistrator<ConcreteCommand>&) = delete;
	CommandRegistrator<ConcreteCommand>& operator=(const CommandRegistrator<ConcreteCommand>&) = delete;
	CommandRegistrator(CommandRegistrator<ConcreteCommand>&&) = delete;
	CommandRegistrator<ConcreteCommand>& operator=(CommandRegistrator<ConcreteCommand>&&) = delete;
	~CommandRegistrator() = default;

private:
	ConcreteCommand command;
};

#include "CommandRegistrator.hpp"

#endif //__COMMAND_REGISTRATOR_HEADER_INCLUDED__