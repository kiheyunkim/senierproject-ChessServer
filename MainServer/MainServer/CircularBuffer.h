#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_
#include "CriticalSection.h"

template<typename T>
class CircularBuffer
{
public:
	using valueType = T;
	
private:
	CriticalSection cs;

private:
	const std::size_t arraySize;
	valueType* dataArray;

private:
	std::size_t front;
	std::size_t rear;
	
public:
	CircularBuffer(std::size_t allocSize)
		:arraySize(allocSize), front(0), rear(0)
	{
		dataArray = new valueType[allocSize];
	}

	~CircularBuffer()
	{
		delete[] dataArray;
	}

public:
	bool isEmpty()
	{
		CriticalSectionSync sync(cs);

		return front == rear;
	}

	bool isFull()
	{
		CriticalSectionSync sync(cs);

		return (front + 1) % arraySize == rear;
	}

	valueType* GetFront()
	{
		CriticalSectionSync sync(cs);

		return &dataArray[front];
	}

	void PopFront()
	{
		CriticalSectionSync sync(cs);

		front = (front + 1) % arraySize;
	}

	void PushBack(const valueType& data)
	{
		CriticalSectionSync sync(cs);

		dataArray[rear] = data;
		//memcpy(&dataArray[rear], &data, sizeof(valueType));
		rear = (rear + 1) % arraySize;
	}
};

#endif //!_CIRCULARBUFFER_H_