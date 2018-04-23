#ifndef __DISTANCE_HEADER_INCLUDED__
#define __DISTANCE_HEADER_INCLUDED__

#include <ostream>

class Distance
{
public:
	static const Distance infinity;

public:
	Distance(unsigned distance);
	Distance(const Distance&) = default;
	Distance(Distance&&) = default;
	Distance& operator=(const Distance&) = default;
	Distance& operator=(Distance&&) = default;
	~Distance() = default;

	Distance& operator=(unsigned distance);

	friend Distance operator+(const Distance& lhs, const Distance& rhs);

	friend std::ostream& operator<<(std::ostream& output, const Distance& distance);
	
	friend bool operator==(const Distance& lhs, const Distance& rhs);
	friend bool operator<(const Distance& lhs, const Distance& rhs);

private:
	void setDistance(unsigned distance);

private:
	unsigned distance;
	bool isInfinity;
};

bool operator!=(const Distance& lhs, const Distance& rhs);
bool operator>(const Distance& lhs, const Distance& rhs);
bool operator<=(const Distance& lhs, const Distance& rhs);
bool operator>=(const Distance& lhs, const Distance& rhs);

#endif //__DISTANCE_HEADER_INCLUDED__