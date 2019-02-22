#ifndef __GRAPH_BUILDER_EXCEPTION_HEADER_INCLUDED__
#define __GRAPH_BUILDER_EXCEPTION_HEADER_INCLUDED__

#include "../../Runtime Error/RuntimeError.h"

namespace GraphIO
{
	class GraphBuilderException : public RuntimeError
	{
	public:
		explicit GraphBuilderException(String errorMessage) :
			RuntimeError(std::move(errorMessage))
		{
		}

		explicit GraphBuilderException(const std::string& errorMessage) :
			RuntimeError(errorMessage)
		{
		}
	};
}

#endif //__GRAPH_BUILDER_EXCEPTION_HEADER_INCLUDED__