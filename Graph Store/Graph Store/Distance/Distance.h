#ifndef __DISTANCE_HEADER_INCLUDED__
#define __DISTANCE_HEADER_INCLUDED__

#include <ostream>

class Distance
{
public:
	static const Distance& getInfinity() noexcept;

public:
	constexpr Distance() noexcept;
	constexpr Distance(unsigned distance) noexcept;

	constexpr Distance& operator+=(const Distance& rhs) noexcept;

	friend std::ostream&
		operator<<(std::ostream& output, const Distance& d);
	friend constexpr bool
		operator==(const Distance& lhs, const Distance& rhs) noexcept;
	friend constexpr bool
		operator<(const Distance& lhs, const Distance& rhs) noexcept;
private:
	static const Distance infinity;

private:
	unsigned distance;
	bool isInfinity;
};

constexpr const Distance
	operator+(const Distance& lhs, const Distance& rhs) noexcept;

constexpr bool operator!=(const Distance& lhs, const Distance& rhs) noexcept;
constexpr bool operator>(const Distance& lhs, const Distance& rhs) noexcept;
constexpr bool operator<=(const Distance& lhs, const Distance& rhs) noexcept;
constexpr bool operator>=(const Distance& lhs, const Distance& rhs) noexcept;

#endif //__DISTANCE_HEADER_INCLUDED__