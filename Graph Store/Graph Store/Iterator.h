#ifndef __ITERATOR_HEADER_INCLUDED__
#define __ITERATOR_HEADER_INCLUDED__

template <class Item>
class Iterator
{
public:
	virtual void advance() = 0;
	virtual bool isValid() const = 0;
	virtual Item& getCurrentItem() = 0;
};

#endif //__ITERATOR_HEADER_INCLUDED__