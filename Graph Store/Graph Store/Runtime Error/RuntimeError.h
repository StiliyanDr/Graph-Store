#ifndef __RUNTIME_ERROR_HEADER_INCLUDED__
#define __RUNTIME_ERROR_HEADER_INCLUDED__

#include <exception>
#include "String/String.h"

class RuntimeError : public std::exception
{
public:
	explicit RuntimeError(String errorMessage) noexcept :
		message(std::move(errorMessage))
	{
	}

	explicit RuntimeError(const std::string& errorMessage) :
		message(errorMessage.c_str())
	{
	}

	const char* what() const override
	{
		return message.cString();
	}

private:
	String message;
};

#endif //__RUNTIME_ERROR_HEADER_INCLUDED__