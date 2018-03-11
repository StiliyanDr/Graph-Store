
template <class T>
inline bool DynamicQueue<T>::isEmpty() const
{
	return list.isEmpty();
}


template <class T>
inline void DynamicQueue<T>::enqueue(const T& item)
{
	list.addBack(item);
}


template <class T>
T DynamicQueue<T>::dequeue()
{
	T firstInQueue(first());
	
	list.removeFront();
	
	return firstInQueue;
}


template <class T>
T DynamicQueue<T>::first() const
{
	if (!isEmpty())
		return list.getFront();
	else
		throw std::logic_error("The queue is empty!");
}
