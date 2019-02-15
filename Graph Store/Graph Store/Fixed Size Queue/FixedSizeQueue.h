#ifndef __FIXED_SIZE_QUEUE_HEADER_INCLUDED__
#define __FIXED_SIZE_QUEUE_HEADER_INCLUDED__

#include "../Dynamic Array/DynamicArray.h"

template <class T>
class FixedSizeQueue
{
public:
	explicit FixedSizeQueue(std::size_t size);
	FixedSizeQueue(const FixedSizeQueue&) = default;
	FixedSizeQueue& operator=(const FixedSizeQueue&) = default;

	void enqueue(const T& item);
	T dequeue();
	T first() const;
	void empty() noexcept;
	bool isEmpty() const noexcept;
	bool isFull() const noexcept;

private:
	std::size_t nextPositionOf(std::size_t position) const;

private:
	DynamicArray<T> slots;
	std::size_t head;
	std::size_t tail;
};

#include "FixedSizeQueue.hpp"

#endif //__FIXED_SIZE_QUEUE_HEADER_INCLUDED__