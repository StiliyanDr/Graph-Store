#ifndef __DISTANCE_ACCESSOR_HEADER_INCLUDED__
#define __DISTANCE_ACCESSOR_HEADER_INCLUDED__

#include "../../Graph/Vertex/Vertex.h"

class DistanceAccessor
{
public:
	const Distance& getKeyOf(const Vertex& vertex) const
	{
		return vertex.getDistance();
	}

	void setKeyOfWith(Vertex& vertex, const Distance& distance) const
	{
		vertex.setDistance(distance);
	}
};

#endif //__DISTANCE_ACCESSOR_HEADER_INCLUDED__