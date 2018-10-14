#ifndef __UNSUPPORTED_ALGORITHM_EXCEPTION_HEADER_INCLUDED__
#define __UNSUPPORTED_ALGORITHM_EXCEPTION_HEADER_INCLUDED__

#include "../../../Runtime Error/RuntimeError.h"

class UnsupportedAlgorithmException : public RuntimeError
{
public:
	explicit UnsupportedAlgorithmException(String errorMessage) :
		RuntimeError(std::move(errorMessage))
	{
	}

	explicit UnsupportedAlgorithmException(const std::string& errorMessage) :
		RuntimeError(errorMessage)
	{
	}
};

#endif //__UNSUPPORTED_ALGORITHM_EXCEPTION_HEADER_INCLUDED__