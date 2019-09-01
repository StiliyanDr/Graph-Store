#ifndef __IS_CONST_ITERATOR_HEADER_INCLUDED__
#define __IS_CONST_ITERATOR_HEADER_INCLUDED__

#include <type_traits>
#include <iterator>

template <class Iterator>
struct IsConstIterator
{
private:
    using Reference = typename std::iterator_traits<Iterator>::reference;
    using ReferencedType = std::remove_reference_t<Reference>;

public:
    static constexpr bool value = std::is_const_v<ReferencedType>;
};

template <class Iterator>
inline constexpr bool isConstIterator = IsConstIterator<Iterator>::value;

#endif //__IS_CONST_ITERATOR_HEADER_INCLUDED__