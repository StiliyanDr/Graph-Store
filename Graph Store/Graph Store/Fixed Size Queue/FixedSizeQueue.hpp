
template <class T>
inline FixedSizeQueue<T>::FixedSizeQueue(std::size_t size) :
	slots(size + 1, size + 1),
	head(0),
	tail(0)
{
}

template <class T>
void FixedSizeQueue<T>::enqueue(const T& item)
{
	if (!isFull())
	{
		slots[tail] = item;
		tail = nextPositionOf(tail);
	}
	else
	{
		throw std::logic_error("The queue is full!");
	}
}

template <class T>
inline bool FixedSizeQueue<T>::isFull() const noexcept
{
	return nextPositionOf(tail) == head;
}

template <class T>
inline std::size_t
FixedSizeQueue<T>::nextPositionOf(std::size_t position) const
{
	return (position + 1) % slots.getSize();
}

template <class T>
T FixedSizeQueue<T>::dequeue()
{
	T firstInQueue = first();
	head = nextPositionOf(head);

	return firstInQueue;
}

template <class T>
T FixedSizeQueue<T>::first() const
{
	if (!isEmpty())
	{
		return slots[head];
	}
	else
	{
		throw std::logic_error("The queue is empty!");
	}
}

template <class T>
inline bool FixedSizeQueue<T>::isEmpty() const noexcept
{
	return head == tail;
}

template <class T>
inline void FixedSizeQueue<T>::empty() noexcept
{
	head = 0;
	tail = 0;
}
