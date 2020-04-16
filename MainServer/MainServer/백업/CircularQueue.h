#ifndef _CIRCULARQUEUE_H_
#define _CIRCULARQUEUE_H_

#include "stdafx.h"

template<typename T>
class CircularQueue
{
public:
	using valueType = T*;
	using queuePointer = T**;

private:
	const std::size_t maxQueueSize;

private:
	std::size_t currentQueueSIze;

private:
	queuePointer queueArray;
	std::size_t frontIndex;
	std::size_t rearIndex;

private:
	void deleteRemainder()
	{
		for (std::size_t i = 0; i < currentQueueSIze; i++)
		{
			delete queueArray[frontIndex];
			frontIndex = (frontIndex + 1) % maxQueueSize;
		}
	}

public:
	bool isFull() { return maxQueueSize - 1 == currentQueueSIze; }
	bool isEmpty() { return currentQueueSIze == 0; }
	void ResetQueue()
	{
		deleteRemainder();					///Clear all data
		delete[] queueArray;

		queueArray = nullptr;				///initialize variables
		frontIndex = rearIndex = 0;

		queueArray = new T[maxQueueSize];	///re-allocate array
	}

	valueType Pop()
	{
		if (isEmpty()) return nullptr;

		valueType temp = queueArray[frontIndex];
		frontIndex = (frontIndex + 1) % maxQueueSize;
		currentQueueSIze--;

		return temp;
	}

	bool Push(valueType object)
	{
		if (isFull()) return false;

		queueArray[rearIndex] = object;
		rearIndex = (rearIndex + 1) % maxQueueSize;
		currentQueueSIze++;

		return true;
	}

public:
	CircularQueue(std::size_t size)
		:currentQueueSIze(0), maxQueueSize(size), queueArray(nullptr), frontIndex(0), rearIndex(0)
	{
		assert(size > 3);

		queueArray = new valueType[maxQueueSize];
		memset(queueArray, 0, maxQueueSize);
		frontIndex = rearIndex = 0;
	}

	~CircularQueue()
	{
		if (!isEmpty())
			deleteRemainder();

		delete[] queueArray;
	}
};


#endif // !_CIRCULARQUEUE_H_