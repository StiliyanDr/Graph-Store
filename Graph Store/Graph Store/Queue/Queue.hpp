#include <stdexcept>

template <class T>
inline void Queue<T>::enqueue(const T& item)
{
	items.addBack(item);
}

template <class T>
T Queue<T>::dequeue()
{
	T firstItem(first());

	items.removeFirst();

	return firstItem;
}

template <class T>
T Queue<T>::first() const
{
	if (!isEmpty())
	{
		return items.getFirst();
	}
	else
	{
		throw std::logic_error("The queue is empty!");
	}
}

template <class T>
inline void Queue<T>::empty()
{
	items.empty();
}

template <class T>
inline bool Queue<T>::isEmpty() const
{
	return items.isEmpty();
}
