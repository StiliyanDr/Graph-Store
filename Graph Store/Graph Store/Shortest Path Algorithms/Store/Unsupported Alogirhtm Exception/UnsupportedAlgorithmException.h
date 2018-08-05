#ifndef __UNSUPPORTED_ALGORITHM_EXCEPTION_HEADER_INCLUDED__
#define __UNSUPPORTED_ALGORITHM_EXCEPTION_HEADER_INCLUDED__

#include <stdexcept>

class UnsupportedAlgorithmException : public std::runtime_error
{
public:
	explicit UnsupportedAlgorithmException(const char* errorMessage);
	explicit UnsupportedAlgorithmException(const std::string& errorMessage);
	virtual ~UnsupportedAlgorithmException() = default;
};

#endif //__UNSUPPORTED_ALGORITHM_EXCEPTION_HEADER_INCLUDED__