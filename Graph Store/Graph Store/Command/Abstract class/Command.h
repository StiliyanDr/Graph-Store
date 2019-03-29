#ifndef __COMMAND_HEADER_INCLUDED__
#define __COMMAND_HEADER_INCLUDED__

#include "../../../../../args-master/args-master/args.hxx"

class String;

class Command
{
public:
	class StringReader
	{
	public:
		void operator()(const std::string& name,
						const std::string& value,
						String& destination) const;
	};

public:
	Command(const Command&) = delete;
	Command& operator=(const Command&) = delete;
	virtual ~Command() = default;

	void execute(args::Subparser& parser);

protected:
	static String getValueOf(args::Positional<String, StringReader>& argument);

protected:
	Command() = default;
	virtual void parseArguments(args::Subparser& parser) = 0;

private:
	virtual void doExecute() = 0;
};

#endif //__COMMAND_HEADER_INCLUDED__