#ifndef __UTILITY_HEADER_INCLUDED__
#define __UTILITY_HEADER_INCLUDED__

#include <utility>

template <class T>
using ConditionalReference = std::conditional_t<
	!std::is_nothrow_move_assignable<T>::value && std::is_copy_assignable<T>::value,
	const T&,
	T&&>;

template <class T>
inline constexpr ConditionalReference<T>
moveAssignIfNoexcept(T& object) noexcept
{
	return static_cast<ConditionalReference<T>>(object);
}

#endif //__UTILITY_HEADER_INCLUDED__