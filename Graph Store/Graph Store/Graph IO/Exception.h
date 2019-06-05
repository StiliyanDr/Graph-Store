#ifndef __GRAPH_IO_EXCEPTION_HEADER_INCLUDED__
#define __GRAPH_IO_EXCEPTION_HEADER_INCLUDED__

#include "Runtime Error/RuntimeError.h"

namespace GraphIO
{
	class Exception : public RuntimeError
	{
	public:
		explicit Exception(String errorMessage) noexcept :
			RuntimeError(std::move(errorMessage))
		{
		}

		explicit Exception(const std::string& errorMessage) :
			RuntimeError(errorMessage)
		{
		}
	};
}

#endif //__GRAPH_IO_EXCEPTION_HEADER_INCLUDED__