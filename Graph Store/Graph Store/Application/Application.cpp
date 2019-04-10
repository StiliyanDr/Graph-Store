#include "Application.h"
#include <stdexcept>
#include <iostream>
#include "String Cutter\StringCutter.h"
#include "Logger\Logger.h"
#include "Command\Graph Command\GraphCommand.h"
#include <windows.h>
#include "Runtime Error\RuntimeError.h"
#include "Graph IO\Directory Loader\DirectoryLoader.h"

Application& Application::instance()
{
	static Application theInstance;
	
	return theInstance;
}

Application::Application() :
	parser("GRAPH STORE APPLICATION"),
	commandsGroup(parser, "SUPPORTED COMMANDS"),
	receivedExitCommand(false)
{
	addExitCommand();
	addHelpCommand();
}

void Application::addExitCommand()
{
	addCommand("exit",
		       "Terminates the program",
		       [&](args::Subparser& parser)
	{
		parser.Parse();
		receivedExitCommand = true;
	});
}

void Application::addHelpCommand()
{
	addCommand("help", "Lists the supported commands",
		       [&](args::Subparser& parser)
	{
		parser.Parse();
		
		std::cout << "Supported commands:\n";

		std::for_each(commands.cbegin(),
			          commands.cend(),
			          [](const args::Command& c)
		{
			std::cout << '\t' << c.Name()
					  << ": " << c.Help() << '\n';
		});
	});
}

void Application::addCommand(const char* name,
							 const char* description,
							 Function f)
{
	verifyStringIsNotNull(name);
	verifyStringIsNotNull(description);

	commands.emplace_front(commandsGroup, name, description, f);
}

void Application::verifyStringIsNotNull(const char* s)
{
	if (s == nullptr)
	{
		throw std::invalid_argument("Expected a string!");
	}
}

void Application::addCommand(const char* name,
							 const char* description,
							 Command& command)
{
	addCommand(name, description, [&](args::Subparser& parser)
	{
		command.execute(parser);
	});
}

void Application::runIn(const String& directory)
{
	try
	{
		setCurrentDirectory(directory);
		loadGraphsFrom(directory);
	}
	catch (std::exception& e)
	{
		Logger::logError(e);
		return;
	}

	passLoadedGraphsToGraphCommands();
	interact();
}

void Application::setCurrentDirectory(const String& path)
{
	bool changedDirectory =
		SetCurrentDirectory(path.cString());

	if (!changedDirectory)
	{
		throw RuntimeError("Could not change the directory to: " + path);
	}
}

void Application::loadGraphsFrom(const String& directory)
{
	GraphIO::DirectoryLoader loader;

	graphs = loader.load(directory);
}

void Application::passLoadedGraphsToGraphCommands()
{
	GraphCommand::setGraphs(graphs);
}

void Application::interact()
{
	const std::size_t BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];

	do
	{
		std::cout << "$ ";
		std::cin.getline(buffer, BUFFER_SIZE);
		invokeCommand(buffer);
	} while (!receivedExitCommand);
}

void Application::invokeCommand(char* commandLine)
{
	std::vector<char> delimiters = { '\'', '\"' };
	StringCutter cutter(delimiters);

	try
	{
		std::vector<std::string> arguments =
			cutter.cutToWords(commandLine);

		parser.ParseArgs(arguments);
	}
	catch (std::exception& e)
	{
		Logger::logError(e);
	}
}
