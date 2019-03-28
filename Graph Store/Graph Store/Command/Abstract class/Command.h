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
	virtual void execute(args::Subparser& parser) = 0;

protected:
	Command() = default;

protected:
	static String getValueOf(args::Positional<String, StringReader>& argument);
};

#endif //__COMMAND_HEADER_INCLUDED__