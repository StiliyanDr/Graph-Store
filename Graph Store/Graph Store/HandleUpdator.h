#ifndef __HANDLE_UPDATOR_HEADER_INCLUDED__
#define __HANDLE_UPDATOR_HEADER_INCLUDED__

#include "Vertex.h"

class HandleUpdator
{
public:
	void operator()(Vertex& vertex, const PriorityQueueHandle& handle) const
	{
		vertex.setPriorityQueueHandle(handle);
	}
};

#endif //__HANDLE_UPDATOR_HEADER_INCLUDED__