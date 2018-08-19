#ifndef __ITERATOR_HEADER_INCLUDED__
#define __ITERATOR_HEADER_INCLUDED__

template <class Item>
class Iterator
{
public:
	virtual ~Iterator() = default;

	virtual Iterator<Item>& operator++() = 0;
	virtual Item* operator->() = 0;
	virtual Item& operator*() = 0;
	virtual bool operator!() const = 0;
	virtual operator bool() const = 0;

protected:
	Iterator() = default;
};

template <class Iterator, class Function>
void forEach(Iterator& iterator, const Function& function)
{
	while (iterator)
	{
		function(*iterator);

		++iterator;
	}
}

#endif //__ITERATOR_HEADER_INCLUDED__