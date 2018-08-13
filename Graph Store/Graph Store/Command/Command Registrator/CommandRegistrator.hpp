#include "../../Application/Application.h"

template <class ConcreteCommand>
CommandRegistrator<ConcreteCommand>::CommandRegistrator(const char* name,
														const char* description)
{
	Application::instance().addCommand(name, description, command);
}
