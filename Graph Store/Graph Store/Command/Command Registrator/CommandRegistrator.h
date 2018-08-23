#ifndef __COMMAND_REGISTRATOR_HEADER_INCLUDED__
#define __COMMAND_REGISTRATOR_HEADER_INCLUDED__

#include "../../Application/Application.h"

template <class ConcreteCommand>
class CommandRegistrator
{
public:
	CommandRegistrator(const char* name, const char* description)
	{
		Application::instance().addCommand(name, description, command);
	}

	CommandRegistrator(const CommandRegistrator<ConcreteCommand>&) = delete;
	CommandRegistrator<ConcreteCommand>& operator=(const CommandRegistrator<ConcreteCommand>&) = delete;
	CommandRegistrator(CommandRegistrator<ConcreteCommand>&&) = delete;
	CommandRegistrator<ConcreteCommand>& operator=(CommandRegistrator<ConcreteCommand>&&) = delete;
	~CommandRegistrator() = default;

private:
	ConcreteCommand command;
};

#endif //__COMMAND_REGISTRATOR_HEADER_INCLUDED__