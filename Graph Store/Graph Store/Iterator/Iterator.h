#ifndef __ITERATOR_HEADER_INCLUDED__
#define __ITERATOR_HEADER_INCLUDED__

template <class Item>
class Iterator
{
public:
	virtual ~Iterator() = default;

	virtual void advance() = 0;
	virtual bool isValid() const = 0;
	virtual Item& getCurrentItem() = 0;

protected:
	Iterator() = default;
};

#endif //__ITERATOR_HEADER_INCLUDED__