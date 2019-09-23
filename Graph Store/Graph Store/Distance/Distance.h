#ifndef __DISTANCE_HEADER_INCLUDED__
#define __DISTANCE_HEADER_INCLUDED__

#include <ostream>

class Distance
{
public:
	static const Distance& getInfinity() noexcept;

public:
	Distance() noexcept;
	Distance(unsigned distance) noexcept;

	Distance& operator+=(const Distance& rhs) noexcept;

	friend std::ostream& operator<<(std::ostream& output,
		                            const Distance& d);
	friend bool operator==(const Distance& lhs,
		                   const Distance& rhs) noexcept;
	friend bool operator<(const Distance& lhs,
		                  const Distance& rhs) noexcept;
private:
	static const Distance infinity;

private:
	unsigned distance;
	bool isInfinity;
};

const Distance operator+(const Distance& lhs,
	                     const Distance& rhs) noexcept;

bool operator!=(const Distance& lhs, const Distance& rhs) noexcept;
bool operator>(const Distance& lhs, const Distance& rhs) noexcept;
bool operator<=(const Distance& lhs, const Distance& rhs) noexcept;
bool operator>=(const Distance& lhs, const Distance& rhs) noexcept;

#endif //__DISTANCE_HEADER_INCLUDED__