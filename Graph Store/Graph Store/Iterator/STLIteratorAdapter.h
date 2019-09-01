#ifndef __STL_ITERATOR_ADAPTER_HEADER_INCLUDED__
#define __STL_ITERATOR_ADAPTER_HEADER_INCLUDED__

#include "Iterator/Iterator.h"
#include "Iterator/IsConstIterator.h"
#include <assert.h>
#include <iterator>

template <class STLIterator>
using STLIteratorAdapterBaseClass =
    AbstractIterator<typename std::iterator_traits<STLIterator>::value_type,
                     isConstIterator<STLIterator>>;

template <class STLIterator>
class STLIteratorAdapter : public STLIteratorAdapterBaseClass<STLIterator>
{
public:
	using typename STLIteratorAdapterBaseClass<STLIterator>::Reference;

public:
	STLIteratorAdapter(STLIterator begin, STLIterator end);
	STLIteratorAdapter& operator++() override;
    const STLIteratorAdapter operator++(int);

protected:
	bool isValid() const override;

private:
	Reference getCurrentItem() const override;

private:
	STLIterator current;
	STLIterator end;
};

template <class STLIterator>
STLIteratorAdapter<STLIterator>::STLIteratorAdapter(STLIterator begin,
                                                    STLIterator end) :
	current(begin),
	end(end)
{
}

template <class STLIterator>
const STLIteratorAdapter<STLIterator>
STLIteratorAdapter<STLIterator>::operator++(int)
{
    auto copy = *this;
    ++(*this);

    return copy;
}

template <class STLIterator>
STLIteratorAdapter<STLIterator>&
STLIteratorAdapter<STLIterator>::operator++()
{
	if (isValid())
	{
		++current;
	}

	return *this;
}

template <class STLIterator>
inline bool
STLIteratorAdapter<STLIterator>::isValid() const
{
	return current != end;
}

template <class STLIterator>
inline auto
STLIteratorAdapter<STLIterator>::getCurrentItem() const
-> Reference
{
	assert(isValid());

	return *current;
}

#endif //__STL_ITERATOR_ADAPTER_HEADER_INCLUDED__