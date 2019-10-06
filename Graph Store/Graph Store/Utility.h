#ifndef __UTILITY_HEADER_INCLUDED__
#define __UTILITY_HEADER_INCLUDED__

#include <utility>

template <class T>
using ConditionalReference = std::conditional_t<
	!std::is_nothrow_move_assignable_v<T> && std::is_copy_assignable_v<T>,
	const T&,
	T&&>;

template <class T>
inline constexpr ConditionalReference<T>
moveAssignIfNoexcept(T& object) noexcept
{
	return static_cast<ConditionalReference<T>>(object);
}

class Identity
{
public:
    template <class T>
    constexpr T&& operator()(T&& x) const noexcept
    {
        return std::forward<T>(x);
    }
};

#endif //__UTILITY_HEADER_INCLUDED__